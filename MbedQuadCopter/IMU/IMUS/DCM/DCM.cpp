#include "DCM.h"
#include "mbed.h"
#include "const.h"
#include "Range.h"
#include "AngleConverters.h"
#include "QuadMaths.h"

//_ki = 0.0087;
  //      _ki_yaw = 0.01;
const float Ki_ROLLPITCH = 0.0087;//0.00002f;
const float Kp_YAW = 1.2f;
const float Ki_YAW = 0.01;//0.00002f;
const float Kp_ROLLPITCH = 0.02f;
const float SPIN_RATE_LIMIT =  20.0f;

// acceleration due to gravity in m/s/s
const float GRAVITY_MSS = 9.80665f;

DCM::DCM(bool useDriftCorrection,
         SensorBase* gyro, 
         SensorBase* accel, 
         MagSensorBase* mag,
         SensorFlightData::SensorFlightDataStruct* flightData, 
         IMUStatus::IMUStatusStruct* imuStatus,
         DebugLoggerBase* debugLogger)
{  
    if (gyro != NULL && accel != NULL && mag != NULL)
    {
        this->gyro = gyro;
        this->accel = accel;
        this->mag = mag;    
    }
    
    if (flightData != NULL)
    {
        this->flightData = flightData->imuFlightData;   
        this->gyroFlightData = flightData->gyroFlightData;
        this->accelFlightData = flightData->accelFlightData;  
        this->magFlightData = flightData->magFlightData; 
    }

    if (imuStatus != NULL)
    {
        this->imuStatus = imuStatus;    
    }
    
    this->debugLogger = debugLogger;
    
  //dcmMatrix[3][3] = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
  this->dcmMatrix[0][0] = 1;
  this->dcmMatrix[0][1] = 0;
  this->dcmMatrix[0][2] = 0;
  this->dcmMatrix[1][0] = 0;
  this->dcmMatrix[1][1] = 1;
  this->dcmMatrix[1][2] = 0;
  this->dcmMatrix[2][0] = 0;
  this->dcmMatrix[2][1] = 0;
  this->dcmMatrix[2][2] = 1;
  
  //updateMatrix[3][3] = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
  this->updateMatrix[0][0] = 0;
  this->updateMatrix[0][1] = 1;
  this->updateMatrix[0][2] = 2;
  this->updateMatrix[1][0] = 3;
  this->updateMatrix[1][1] = 4;
  this->updateMatrix[1][2] = 5;
  this->updateMatrix[2][0] = 6;
  this->updateMatrix[2][1] = 7;
  this->updateMatrix[2][2] = 8;

  //temporaryMatrix[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  this->temporaryMatrix[0][0] = 0;
  this->temporaryMatrix[0][1] = 0;
  this->temporaryMatrix[0][2] = 0;
  this->temporaryMatrix[1][0] = 0;
  this->temporaryMatrix[1][1] = 0;
  this->temporaryMatrix[1][2] = 0;
  this->temporaryMatrix[2][0] = 0;
  this->temporaryMatrix[2][1] = 0;
  this->temporaryMatrix[2][2] = 0;
  
  this->errorRollPitch[0] = 0;
  this->errorRollPitch[1] = 0;
  this->errorRollPitch[2] = 0;
  
  this->errorYaw[0] = 0;
  this->errorYaw[1] = 0;
  this->errorYaw[2] = 0;
  
  this->omegaP[0] = 0;
  this->omegaP[1] = 0;
  this->omegaP[2] = 0;
  
  this->omegaI[0] = 0;
  this->omegaI[1] = 0;
  this->omegaI[2] = 0;
  this->omegaISum.zero();
  this->raSum.zero();
  this->_omega_I_sum_time = 0.0f;
  this->omega[0] = 0;
  this->omega[1] = 0;
  this->omega[2] = 0;
  
  this->omegaVector[0] = 0;
  this->omegaVector[1] = 0;
  this->omegaVector[2] = 0;
  
  this->useDriftCorrection = useDriftCorrection;
  this->gyroVector[0] = 0.0;
  this->gyroVector[1] = 0.0;
  this->gyroVector[2] = 0.0;
  this->accelVector[0] = 0.0;
  this->accelVector[1] = 0.0;
  this->accelVector[2] = 0.0;
  this->dcm_TimeDelta = 0.0;
}

void DCM::Init()
{
    
}
void DCM::UpdateEulerAngles(float timeDelta)
{
    this->UpdateHeading(timeDelta);
    
    if (this->flightData != NULL)
    {
        this->dcmMatrix.to_euler(&roll, &pitch, &yaw);
        
        this->flightData->pitchAngle = AngleConverters::RadiansToDegrees(pitch);
        this->flightData->rollAngle  = AngleConverters::RadiansToDegrees(roll);
        this->flightData->yawAngle  = AngleConverters::RadiansToDegrees(yaw);
        this->flightData->pitchCentiAngle = AngleConverters::DegreesToCentiDegrees(this->flightData->pitchAngle);
        this->flightData->rollCentiAngle = AngleConverters::DegreesToCentiDegrees(this->flightData->rollAngle);
        this->flightData->yawCentiAngle = AngleConverters::DegreesToCentiDegrees(this->flightData->yawAngle);
        
        this->UpdateTrigRollPitchYaw();
        
        if (this->debugLogger != NULL)
        {
            char debugMsg[200];
            
            sprintf(&debugMsg[0],"IMU Pitch <%f>",this->flightData->pitchAngle);
            //IMU Roll <%f>\nIMU Yaw <%f>\n\n\0", this->flightData->pitchAngle, this->flightData->rollAngle , this->flightData->yawAngle);
            this->debugLogger->Log(&debugMsg[0]);
        }
    }
    else
    {
        if (this->imuStatus != NULL)
        {
            this->imuStatus->readyForUse = false;   
        }
        
        this->CallCriticalStop(); 
    }
}

void DCM::UpdateTrigRollPitchYaw()
{
    Vector2f yaw_vector;
    
    if (this->flightData != NULL)
    {
        const Matrix3f &temp = this->dcmMatrix;

        // sin_yaw, cos_yaw
        yaw_vector.x = temp.a.x;
        yaw_vector.y = temp.b.x;
        yaw_vector.normalize();
        this->flightData->sinYaw = Range::ConstrainFloat(yaw_vector.y, -1.0, 1.0);
        this->flightData->cosYaw = Range::ConstrainFloat(yaw_vector.x, -1.0, 1.0);

        // cos_roll, cos_pitch
        this->flightData->cosPitch = QuadMaths::SafeSqrt(1 - (temp.c.x * temp.c.x));
        this->flightData->cosRoll = temp.c.z / this->flightData->cosPitch;
        this->flightData->cosPitch = Range::ConstrainFloat(this->flightData->cosPitch, 0, 1.0);
        this->flightData->cosRoll = Range::ConstrainFloat(this->flightData->cosRoll, -1.0, 1.0); // this relies on constrain_float() of infinity doing the right thing,which it does do in avr-libc

        // sin_roll, sin_pitch
        this->flightData->sinPitch = -temp.c.x;
        this->flightData->sinRoll = temp.c.y / this->flightData->cosPitch;
    }
    else
    {
        this->CallCriticalStop();
    }  
}

void DCM::UpdateHeading(float timeDelta)
{
    this->gyro->GetScaledSensorData();
    this->accel->GetScaledSensorData();
    this->mag->UpdateHeadingScaled(AngleConverters::DegreesToRadians(this->flightData->rollAngle),
                                   AngleConverters::DegreesToRadians(this->flightData->pitchAngle));
    if (this->flightData != NULL)
    {
        if (this->gyroFlightData != NULL && this->accelFlightData != NULL &&
            this->magFlightData != NULL)
        {
            if (timeDelta <= 0.2f)
            {
                gyroVector[0] = this->gyroFlightData->scaledXAxisRate;
                gyroVector[1] = this->gyroFlightData->scaledYAxisRate;
                gyroVector[2] = this->gyroFlightData->scaledZAxisRate;
  
                accelVector[0] = this->accelFlightData->scaledXAxis;
                accelVector[1] = this->accelFlightData->scaledYAxis;
                accelVector[2] = this->accelFlightData->scaledZAxis;
            
               
                this->dcm_TimeDelta =timeDelta;
  
                this->Matrix_update(gyroVector,timeDelta);
                this->Normalize();
                this->Drift_correction(accelVector,this->magFlightData->scaledHeading);
                //this->DriftCorrection(accelVector,this->magFlightData->scaledHeading, timeDelta);
            }
            else
            {
                this->raSum.zero();
                this->raDeltat = 0.0f;
            }
        }
        else
        {
            this->CallCriticalStop();   
        }
    }
    else
    {
        if (this->imuStatus != NULL)
        {
            this->imuStatus->readyForUse = false;   
        }
        
        this->CallCriticalStop();
    }
}

float DCM::PGain(float spinRate)
{
    if (spinRate < AngleConverters::DegreesToRadians(50)) {
        return 1.0f;
    }
    if (spinRate > AngleConverters::DegreesToRadians(500)) {
        return 10.0f;
    }
    
    return spinRate/AngleConverters::DegreesToRadians(50);
}

float DCM::YawGain(void) const
{
    float VdotEFmag = QuadMaths::pythagorous2(accelEf.x,accelEf.y);
    
    if (VdotEFmag <= 4.0f) 
    {
        return 0.2f*(4.5f - VdotEFmag);
    }
    
    return 0.1f;
}

void DCM::CallCriticalStop()
{
    if (this->ptr != NULL && this->method_callback != NULL)
    {
        (this->ptr->*this->method_callback)();   
    } 
}

float DCM::YawErrorCompass(void)
{
    const Vector3f mag(this->magFlightData->scaledXAxis,
                       this->magFlightData->scaledYAxis,
                       this->magFlightData->scaledZAxis);
    
    float cos_pitch_sq = 1.0f-(this->dcmMatrix.c.x*this->dcmMatrix.c.x);

    // Tilt compensated magnetic field Y component:
    float headY = mag.y * this->dcmMatrix.c.z - mag.z * this->dcmMatrix.c.y;

    // Tilt compensated magnetic field X component:
    float headX = mag.x * cos_pitch_sq - this->dcmMatrix.c.x * (mag.y * this->dcmMatrix.c.y + mag.z * this->dcmMatrix.c.z);

    // magnetic heading
    // 6/4/11 - added constrain to keep bad values from ruining DCM Yaw - Jason S.
    float heading = Range::ConstrainFloat(atan2f(-headY,headX), -3.15f, 3.15f);
    
        
    // get the mag vector in the earth frame
    Vector2f rb = this->dcmMatrix.mulXY(mag);

    rb.normalize();
    if (rb.is_inf()) {
        // not a valid vector
        return 0.0;
    }

  
    magEarth.x = cosf(heading);
    magEarth.y = sinf(heading);
    

    // calculate the error term in earth frame
    // calculate the Z component of the cross product of rb and _mag_earth
    return rb % magEarth;
}

void DCM::DriftCorrectionYaw(float updateDelta)
{
    float yawError;
    float errorZ;
    float spinRate;
    
    yawError = this->YawErrorCompass();
    
    // convert the error vector to body frame
    errorZ = this->dcmMatrix.c.z * yawError;
    
    // the spin rate changes the P gain, and disables the
    // integration at higher rates
    spinRate = this->omega.length();
    
    this->omegaPYaw.z = errorZ * this->PGain(spinRate) * Kp_YAW * this->YawGain();
    
    if (updateDelta < 2.0f && spinRate < AngleConverters::DegreesToRadians(SPIN_RATE_LIMIT)) {
        // also add to the I term
        omegaISum.z += errorZ * Ki_YAW * updateDelta;
    }
}

bool DCM::Renorm (Vector3f const &a, Vector3f &result)
{
    float renorm_val;
    
    renorm_val = 1.0f / a.length(); 
    
    if (!(renorm_val < 2.0f && renorm_val > 0.5f)) {
        // this is larger than it should get - log it as a warning
        if (!(renorm_val < 1.0e6f && renorm_val > 1.0e-6f)) {
            // we are getting values which are way out of
            // range, we will reset the matrix and hope we
            // can recover our attitude using drift
            // correction before we hit the ground!
            //Serial.printf("ERROR: DCM renormalisation error. renorm_val=%f\n",
            //     renorm_val);
            return false;
        }
    }

    result = a * renorm_val;
    return true;
}

void DCM::Normalize(void)
{
  float error=0;
  Vector3f t0, t1, t2;
  float renorm=0;
  
  error= -(this->dcmMatrix.a*this->dcmMatrix.b)*.5; //eq.19

  t0 = this->dcmMatrix.b* error; //eq.19
  
  t1 = this->dcmMatrix.a*error; //eq.19
  
  t0 += this->dcmMatrix.a; //eq.19
  t1 += this->dcmMatrix.b; //eq.19
  
  t2 = t0 % t1; //eq.20
  
  this->Renorm(t0, this->dcmMatrix.a);
  this->Renorm(t1, this->dcmMatrix.b);
  this->Renorm(t2, this->dcmMatrix.c);
        
  /*renorm= .5 *(3 - (t0*t0)); //eq.21
  this->dcmMatrix.a = t0 * renorm;
  
  renorm= .5 *(3 - (t1*t1)); //eq.21
  this->dcmMatrix.b = t1 * renorm;
  
  renorm= .5 *(3 - (t2*t2)); //eq.21
  this->dcmMatrix.c = t2 * renorm;*/
}

void DCM::DriftCorrection(Vector3f accelVector, float magHeading, float timeDelta)
{
    this->DriftCorrectionYaw(timeDelta);
    
    //Now Roll Pitch correction;
    
    // rotate accelerometer values into the earth frame
    accelEf = this->dcmMatrix * accelVector;
    
    // integrate the accel vector in the earth frame between GPS readings
    raSum += accelEf * timeDelta;
  

    // keep a sum of the deltat values, so we know how much time
    // we have integrated over
    raDeltat += timeDelta;
    
    if (this->raDeltat >= 0.2f)
    {
        //Update drift
        // equation 9: get the corrected acceleration vector in earth frame. Units
        // are m/s/s
        Vector3f GA_e;
        GA_e = Vector3f(0, 0, -1.0f);


        float ra_scale = 1.0f/(this->raDeltat*GRAVITY_MSS);
        
        
        // calculate the error term in earth frame.
        // we do this for each available accelerometer then pick the
        // accelerometer that leads to the smallest error term. This takes
        // advantage of the different sample rates on different
        // accelerometers to dramatically reduce the impact of aliasing
        // due to harmonics of vibrations that match closely the sampling
        // rate of our accelerometers. On the Pixhawk we have the LSM303D
        // running at 800Hz and the MPU6000 running at 1kHz, by combining
        // the two the effects of aliasing are greatly reduced.
        Vector3f error;
        Vector3f GA_b;
        
        this->raSum *= ra_scale;
        GA_b = this->raSum;
        
        if (!GA_b.is_zero())
        {
            GA_b.normalize();
            
            if (!GA_b.is_inf()) 
            {

            
                error = GA_b % GA_e;
                float error_length = error.length();
                error.z = 0;
                error = this->dcmMatrix.mul_transpose(error);
                
                if (!error.is_nan() || !error.is_inf())
                {
                    
                    // base the P gain on the spin rate
                    float spin_rate = this->omega.length();
                    
                    // we now want to calculate _omega_P and _omega_I. The
                    // _omega_P value is what drags us quickly to the
                    // accelerometer reading.
                    this->omegaP = error * this->PGain(spin_rate) * Kp_ROLLPITCH;
                    
                    // accumulate some integrator error
                    if (spin_rate < AngleConverters::DegreesToRadians(SPIN_RATE_LIMIT)) 
                    {
                        this->omegaISum += error * Ki_ROLLPITCH * raDeltat;
                        _omega_I_sum_time += raDeltat;
                    }

                    if (_omega_I_sum_time >= 5) 
                    {
                        this->omegaI += this->omegaISum;
    
                        this->omegaISum.zero();
                        _omega_I_sum_time = 0.0f;
                        
                        this->raSum.zero();
                        this->raDeltat = 0.0f;
                    }
                }
            }
        }
    }
}

void DCM::Drift_correction(Vector3f accelVector, float magHeading)
{
  float mag_heading_x;
  float mag_heading_y;
  float errorCourse;
  //Compensation the Roll, Pitch and Yaw drift. 
  
  float Accel_magnitude;
  float Accel_weight;
  
  
  //*****Roll and Pitch***************

  // Calculate the magnitude of the accelerometer vector
  Accel_magnitude = sqrtf(accelVector[0]*accelVector[0] + accelVector[1]*accelVector[1] + accelVector[2]*accelVector[2]);
  Accel_magnitude = Accel_magnitude / PhysicalConstants::Gravity; // Scale to gravity.
  // Dynamic weighting of accelerometer info (reliability filter)
  // Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
  Accel_weight = Range::Constrain(1 - 2*fabs(1 - Accel_magnitude),0.0f,1.0f);  //

  this->errorRollPitch = accelVector % this->dcmMatrix.c; //VectorMaths::Vector_Cross_Product(&this->errorRollPitch[0],&accelVector[0],&this->dcmMatrix[2][0]); //adjust the ground of reference
  this->omegaP = this->errorRollPitch * (Kp_ROLLPITCH*Accel_weight);//VectorMaths::Vector_Scale(&this->omegaP[0],&this->errorRollPitch[0],Kp_ROLLPITCH*Accel_weight);
  
  Scaled_Omega_I = this->errorRollPitch * (Ki_ROLLPITCH*Accel_weight);// VectorMaths::Vector_Scale(&Scaled_Omega_I[0],&this->errorRollPitch[0],Ki_ROLLPITCH*Accel_weight);
  this->omegaI += Scaled_Omega_I;//VectorMaths::Vector_Add(&this->omegaI[0],this->omegaI,Scaled_Omega_I);     
  
  
  
  // We make the gyro YAW drift correction based on compass magnetic heading
 
  mag_heading_x = cos(magHeading);
  mag_heading_y = sin(magHeading);
  
  errorCourse= (this->dcmMatrix[0][0]*mag_heading_y) - (this->dcmMatrix[1][0]*mag_heading_x);  //Calculating YAW error
  this->errorYaw = this->dcmMatrix.c * errorCourse;//VectorMaths::Vector_Scale(this->errorYaw,&this->dcmMatrix[2][0],errorCourse); //Applys the yaw correction to the XYZ rotation of the aircraft, depeding the position.
  
  Scaled_Omega_P = this->errorYaw * Kp_YAW;//VectorMaths::Vector_Scale(&Scaled_Omega_P[0],&errorYaw[0],Kp_YAW);//.01proportional of YAW.
  this->omegaP += Scaled_Omega_P;//VectorMaths::Vector_Add(&this->omegaP[0],this->omegaP,Scaled_Omega_P);//Adding  Proportional.
  
  Scaled_Omega_I = errorYaw * Ki_YAW;//VectorMaths::Vector_Scale(&Scaled_Omega_I[0],&errorYaw[0],Ki_YAW);//.00001Integrator
  this->omegaI += Scaled_Omega_I;//VectorMaths::Vector_Add(&this->omegaI[0],this->omegaI,Scaled_Omega_I);//adding integrator to the Omega_I
  
}

void DCM::Matrix_update(Vector3f gyro, float timeDelta)
{
    this->omega = gyro + this->omegaI;//VectorMaths::Vector_Add(&this->omega[0], &gyro[0], &this->omegaI[0]);  //adding proportional term
    this->omegaVector = this->omega + this->omegaP + this->omegaPYaw;//VectorMaths::Vector_Add(&this->omegaVector[0], &this->omega[0], &this->omegaP[0]); //adding Integrator term
  
    
    if (!this->useDriftCorrection) // Do not use drift correction
    {
       /* this->updateMatrix[0][0]=0;
        this->updateMatrix[0][1]=-timeDelta*gyro[2];//-z
        this->updateMatrix[0][2]=timeDelta*gyro[1];//y
        this->updateMatrix[1][0]=timeDelta*gyro[2];//z
        this->updateMatrix[1][1]=0;
        this->updateMatrix[1][2]=-timeDelta*gyro[0];
        this->updateMatrix[2][0]=-timeDelta*gyro[1];
        this->updateMatrix[2][1]=timeDelta*gyro[0];
        this->updateMatrix[2][2]=0;
        
        this->dcmMatrix += this->dcmMatrix*this->updateMatrix;    
        */
        this->dcmMatrix.rotate((gyro*timeDelta));
    }
    else
    {
        this->dcmMatrix.rotate((this->omegaVector*timeDelta));
        
        /*
        //temp_matrix.a.x = a.y * g.z - a.z * g.y;
        this->updateMatrix[0][0]=0;
        this->updateMatrix[0][1]=-timeDelta*this->omegaVector[2];//-z
        this->updateMatrix[0][2]=timeDelta*this->omegaVector[1];//y
        this->updateMatrix[1][0]=timeDelta*this->omegaVector[2];//z
        this->updateMatrix[1][1]=0;
        this->updateMatrix[1][2]=-timeDelta*this->omegaVector[0];//-x
        this->updateMatrix[2][0]=-timeDelta*this->omegaVector[1];//-y
        this->updateMatrix[2][1]=timeDelta*this->omegaVector[0];//x
        this->updateMatrix[2][2]=0;*/
        
    }
    
   
}

// Init rotation matrix using euler angles
void DCM::InitRotationMatrix(float yaw, float pitch, float roll)
{
    this->dcmMatrix.from_euler(roll,pitch,yaw);
}

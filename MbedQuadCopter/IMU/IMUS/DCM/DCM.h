#ifndef DCM_H
#define DCM_H
#include "IMUBase.h"
#include "IMUFlightData.h"
#include "GyroFlightData.h"
#include "AccelFlightData.h"
#include "MagFlightData.h"
#include "SensorFlightData.h"
#include "IMUStatus.h"
#include "Matrix3.h"
#include "Vector3.h"
#include "Vector2.h"
#include "DebugLoggerBase.h"
#include "SensorBase.h"
#include "MagSensorBase.h"

class DCM : public IMUBase
{
  public:
        DCM(bool useDriftCorrection,
            SensorBase* gyro, 
            SensorBase* accel, 
            MagSensorBase* mag, 
            SensorFlightData::SensorFlightDataStruct* flightData,
            IMUStatus::IMUStatusStruct* imuStatus,
            DebugLoggerBase* debugLogger);
        void InitRotationMatrix(float yaw, float pitch, float roll);
        virtual void Init();
        virtual void UpdateEulerAngles(float timeDelta);
    public:
        template <class ClassT>
        void CriticalStopCallbackHandler(ClassT *ptr, void(ClassT::*pFunc)())
        {
                this->ptr = (FunctionPtrInternal*)ptr;  
                this->method_callback = (void (FunctionPtrInternal::*)(void))pFunc; 
        }
    private:
        void UpdateHeading(float timeDelta);
        FunctionPtrInternal* ptr;
        void (FunctionPtrInternal::*method_callback)(void);
        bool Renorm (Vector3f const &a, Vector3f &result);
        float YawErrorCompass(void);
        void DriftCorrectionYaw(float updateDelta);
        float PGain(float spinRate);
        float YawGain(void) const;
  private:  
        void Normalize(void);
        void Drift_correction(Vector3f accelVector, float magHeading);
        void DriftCorrection(Vector3f accelVector, float magHeading, float timeDelta);
        void Matrix_update(Vector3f gyro, float timeDelta);
        void UpdateTrigRollPitchYaw();
    
  private:
        Matrix3f dcmMatrix;
        Matrix3f updateMatrix;// = {{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
        Matrix3f temporaryMatrix;// = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
        Vector3f errorRollPitch;// = {0, 0, 0};  
        Vector3f errorYaw;// = {0, 0, 0};
        Vector3f omegaP;//= {0, 0, 0}; // Omega Proportional correction
        Vector3f omegaPYaw;
        Vector3f omegaI;//= {0, 0, 0}; // Omega Integrator
        Vector3f omegaISum;
        Vector3f omega;//= {0, 0, 0};
        Vector3f omegaVector;//= {0, 0, 0}; // Corrected Gyro_Vector data
        Vector3f Scaled_Omega_P;
        Vector3f Scaled_Omega_I;
        Vector3f gyroVector;
        Vector3f accelVector;
        Vector2f magEarth;
        Vector3f accelEf;
        Vector3f raSum;
        float raDeltat;
        float dcm_TimeDelta;
        float _omega_I_sum_time;
  private:
        bool useDriftCorrection;
        IMUFlightData::IMUFlightDataStruct* flightData;
        GyroFlightData::GyroFlightDataStruct* gyroFlightData;
        AccelFlightData::AccelFlightDataStruct* accelFlightData;
        MagFlightData::MagFlightDataStruct* magFlightData;
        IMUStatus::IMUStatusStruct* imuStatus;
        virtual void CallCriticalStop();
        float roll, pitch, yaw;
        DebugLoggerBase* debugLogger;
        SensorBase* gyro; 
        SensorBase* accel; 
        MagSensorBase* mag;
        
};

#endif
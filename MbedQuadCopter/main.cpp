#include "SystemIncludes.h"
#include "SystemCalibControllersIncludes.h"
#include "SystemConfigControllersIncludes.h"
#include "SystemHighLevelRateControllerIncludes.h"
#include "SystemDataStorageControllerIncludes.h"
#include "SystemBusControllersIncludes.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Data structures
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
__attribute((section("AHBSRAM1")))
FlightCtrlData::FlightCtrlDataStruct flightCtrlData;

__attribute((section("AHBSRAM1"))) 
SystemStatus::SystemStatusStruct quadSystemStatus;

__attribute((section("AHBSRAM1"))) 
SensorFlightData::SensorFlightDataStruct quadFlightData;

__attribute((section("AHBSRAM1"))) 
AccelFlightData::AccelFlightDataStruct accelFlightData;

__attribute((section("AHBSRAM1"))) 
AltMeterFlightData::AltMeterFlightDataStruct altMeterFlightData;

__attribute((section("AHBSRAM1"))) 
GyroFlightData::GyroFlightDataStruct gyroFlightData;

__attribute((section("AHBSRAM1"))) 
MagFlightData::MagFlightDataStruct magFlightData;

__attribute((section("AHBSRAM1"))) 
IMUFlightData::IMUFlightDataStruct imuFlightData;

__attribute((section("AHBSRAM1"))) 
GPSPositionData::GPSPositionDataStruct gpsPositionData;

__attribute((section("AHBSRAM1")))
FlightPack3CellData::FlightPack3CellDataStruct flightPackData;

__attribute((section("AHBSRAM1")))
QuadMotorConfigFlightData::QuadMotorConfigFlightDataStruct motorConfig;
                    
__attribute((section("AHBSRAM1")))
QuadMotorProperties::GlobalLimits gloabalLimits;

__attribute((section("AHBSRAM1")))
FlightMode::FlightModeStruct flightSystemMode;

__attribute((section("AHBSRAM1")))
MotionControlFlightData::MotionControlFlightDataStruct motionCtrl;

__attribute((section("AHBSRAM1")))
PIDControlData::PIDControlDataStruct rateRollPIDCtrl;

__attribute((section("AHBSRAM1")))
PIDControlData::PIDControlDataStruct ratePitchPIDCtrl;

__attribute((section("AHBSRAM1")))
PIDControlData::PIDControlDataStruct rateYawPIDCtrl;

__attribute((section("AHBSRAM1")))
PControlData::PControlDataStruct stabalisedRollCtrl;

__attribute((section("AHBSRAM1")))
PControlData::PControlDataStruct stabalisedPitchCtrl;

__attribute((section("AHBSRAM1")))
PControlData::PControlDataStruct stabalisedYawCtrl;

__attribute((section("AHBSRAM0")))
UserResponce::UserResponceStruct userResponce;

__attribute((section("AHBSRAM1")))
DataRequestRecv::DataRequestRecvs dataRequest;

__attribute((section("AHBSRAM1")))
SystemStatusAck::SystemStatusAckStruct systemStatusAck;

__attribute((section("AHBSRAM1")))
SystemConfigurators::SystemConfiguratorStruct sysConfigurator;

__attribute((section("AHBSRAM1")))
SensorCalibration::SensorCalibratorStruct sysCalibrators;

__attribute((section("AHBSRAM1")))
SPIBusCtrlTypes::IsSPIBusUser isSPIBusUser;

__attribute((section("AHBSRAM1")))
GyroTypes::GyroCalData gyroCalData = {-34.82,100.41,-16.38};

__attribute((section("AHBSRAM1")))
XYZAxisMinMax::XYZAxisMinMaxCalData accelCalData = {-278,270,-254,254,-294,235};

__attribute((section("AHBSRAM1")))
XYZAxisMinMax::XYZAxisMinMaxCalData magCalData = {-511,581,-516,568,-489,486};

__attribute((section("AHBSRAM1")))
MPL115A2CalData::CalData AltCalData = {1000.0,0.0};

__attribute((section("AHBSRAM1")))
SysCtrlConfig::SysCtrlConfigStruct sysCtrlConfig;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Controllers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
__attribute((section("AHBSRAM0")))
BinaryParser binaryDataPckParser;

//Create PID entites
__attribute((section("AHBSRAM0")))
PID rateRollPID = PID(&rateRollPIDCtrl, Vector2f(-100.0,100.0));

__attribute((section("AHBSRAM0")))
PID ratePitchPID = PID(&ratePitchPIDCtrl, Vector2f(-100.0,100.0));

__attribute((section("AHBSRAM0")))
PID rateYawPID = PID(&rateYawPIDCtrl, Vector2f(-100.0,100.0));

//Create P entities
__attribute((section("AHBSRAM0")))
P stabalisedRollP = P(&stabalisedRollCtrl);

__attribute((section("AHBSRAM0")))
P stabalisedPitchP = P(&stabalisedPitchCtrl);

__attribute((section("AHBSRAM0")))
P stabalisedYawP = P(&stabalisedYawCtrl);

__attribute((section("AHBSRAM0")))
PIDAxisRateConfiguration::PIDAxisRateConfigStruct RatePIDCtrls = {&rateRollPID,&ratePitchPID,&rateYawPID};

__attribute((section("AHBSRAM0")))
PAxisStabaliseConfiguration::PAxisStabaliseConfigStruct stabalisedPCtrls = {&stabalisedRollP,&stabalisedPitchP,&stabalisedYawP};

//Create rate controller
__attribute((section("AHBSRAM0")))
BasicRateController* defauleLowLevelRateCtrl = new BasicRateController (&RatePIDCtrls,
                              &motionCtrl,
                              &quadFlightData,
                              sysCtrlConfig.motorController);
                              
__attribute((section("AHBSRAM0")))                              
BasicLowLevelRateController* lowLevelRateController = defauleLowLevelRateCtrl;

//Create rate controller high level
__attribute((section("AHBSRAM0")))
BaseHighLevelRateController* highLevelRateController = new DefaultHighLevelController(defauleLowLevelRateCtrl,&flightCtrlData);

__attribute((section("AHBSRAM0")))
SensorCalBase* gyroCalObj = new ITG3200Cal(&gyroCalData, &quadSystemStatus.gyroStatus);

__attribute((section("AHBSRAM0")))
SensorCalBase* accelCalObj = new ADXL345Cal(&accelCalData, &quadSystemStatus.accelStatus);

__attribute((section("AHBSRAM0")))
SensorCalBase* magCalObj = new HMC5883LCal(&magCalData, &quadSystemStatus.magStatus);

__attribute((section("AHBSRAM0")))
AltimeterCalBase* altimeterCalObj = new MPL115A2Cal(&AltCalData, &quadSystemStatus.altStatus);

__attribute((section("AHBSRAM0")))
SerDebugLogger* seriallogger = new SerDebugLogger();

__attribute((section("AHBSRAM0")))
SystemModeControllerBase* modeController = new BasicSystemModeController(&flightSystemMode);


int main() 
{
    CmdMsgControllerBase* cmdMsgController;
    SchedulerBase* taskScheduler;
    SystemTaskModeControllerBase* modeToTaskController;
    FlightDataBinaryEncoderDecoder* flightDataEncoder;
    EncodersDecodersBase* systemStatusEncoder;
    SysCtrlConfigCtrl* systemConfig;
    
    //Setup Bus Controllers
    ConfigureSPIBusUsers::SetupSPIBusUserMapping(&isSPIBusUser);
    I2C bus1(p9, p10);
    I2C bus2(P0_27, P0_28);
    I2CBusCtrl* i2cBusCtrl1 = new I2CBusCtrl(5, &bus1);
    I2CBusCtrl* i2cBusCtrl2 = new I2CBusCtrl(5, &bus2);
    SPI spiBus(p5,p6,p7);
    SPIBusCtrl* spiBusCtrl = new SPIBusCtrl(5, &spiBus, &isSPIBusUser);
    
    quadSystemStatus.heartBeatStatus.HeartBeatReceived = false;
    HeartBeatMonitor* heartBeatMonitor = new HeartBeatMonitor(&quadSystemStatus.heartBeatStatus, &quadSystemStatus.commsStatus, seriallogger);
    
    seriallogger->EnableDisableLogger(true);
    
    //Initialise the system ctrl config struct;
    SysCtrlConfig::InitialiseSysCtrlConfig(&sysCtrlConfig);
    
    //Setup data storage controller
    sysCtrlConfig.dataStorageController = new SDCardStorageController(spiBusCtrl);
    
    //Setup heartBeat indicator
    HeartBeatIndicator heartBeat(LED1);
    
    //Set system comms
    //systemComms = new Xbee(p13, p14, p30);
    quadSystemStatus.timeSynchStatus.timeSynched = false;
    quadSystemStatus.commsStatus.mode = CommTypes::Synching;
    sysCtrlConfig.systemComms = new Xbee(p13, p14, p30, seriallogger, &quadSystemStatus.commsStatus);
    sysCtrlConfig.systemComms->EstablishComms();
    heartBeatMonitor->SendRequest(sysCtrlConfig.systemComms, &BaseComms::SendDataPacket);
    
    //Setup flightdata encoder
    flightDataEncoder = new FlightDataBinaryEncoderDecoder(&quadFlightData);
    flightDataEncoder->SendRequest(sysCtrlConfig.systemComms, &BaseComms::SendDataPacket);
    
    //Setup System Status encoder
    systemStatusEncoder = new StatusDataTextEncoder(&quadSystemStatus, sysCtrlConfig.systemComms);
    
    //Configure bus users
    I2CBusCtrlUser::I2CBusUserConfig imuBusUser;
    imuBusUser.busFrequency = 400000;
    I2CBusCtrlUser::I2CBusUserConfig altBusUser;
    altBusUser.busFrequency = 400000;
    I2CBusCtrlUser::I2CBusUserConfig flightPackChkBusUser;
    flightPackChkBusUser.busFrequency = 400000;
    I2CBusCtrlUser::I2CBusUserConfig pwmBusUser;
    pwmBusUser.busFrequency = 400000;

    //Add bus users to bus controller
    i2cBusCtrl1->AddBusUser(BusCtrlUsers::IMU,&imuBusUser);
    i2cBusCtrl2->AddBusUser(BusCtrlUsers::IMU,&imuBusUser);
    i2cBusCtrl1->AddBusUser(BusCtrlUsers::Alt,&altBusUser);
    i2cBusCtrl1->AddBusUser(BusCtrlUsers::FlightPackChecker,&flightPackChkBusUser);
    i2cBusCtrl1->AddBusUser(BusCtrlUsers::PWMCtrl,&pwmBusUser);
    
    //Setup configuration and calibration structure mappings.
    sysCalibrators.gyroCal = gyroCalObj;
    sysCalibrators.gyroCalData = &gyroCalData;
    
    sysCalibrators.accelCal = accelCalObj;
    sysCalibrators.accelCalData = &accelCalData;
    
    sysCalibrators.magCal = magCalObj;
    sysCalibrators.magCalData = &magCalData;
    
    sysCalibrators.altCal = altimeterCalObj;
    sysCalibrators.altCalData = &AltCalData;
    
    sysCalibrators.userResponce = &userResponce;
    sysCtrlConfig.sensorCalibrators = &sysCalibrators;
    
    
    //Initialise rate control flight data
    SetupMotionCtrl::Configure(&motionCtrl, 
                                &rateRollPIDCtrl,
                                &ratePitchPIDCtrl,
                                &rateYawPIDCtrl,
                                &stabalisedRollCtrl,
                                &stabalisedRollCtrl,
                                &stabalisedRollCtrl);
               
    //Initialise system flight data
    FlightDataSetup::Configure(&quadFlightData,
                               &accelFlightData,
                               &altMeterFlightData,
                               &gyroFlightData,
                               &magFlightData,
                               &imuFlightData,
                               &flightPackData, 
                               &gpsPositionData);
                               
    FlightDataSetup::Initialise(&quadFlightData);

    FlightDataSetup::Initialise(&userResponce, &dataRequest,&systemStatusAck);
    
    //Setup Motor flight structs
    motorConfig.gloabalLimits = &gloabalLimits;
    
    //Initialise system status data
    SystemStatusSetup::Initialise(&quadSystemStatus);
    
    
    //Setup system configuration controller
    systemConfig = new SysCtrlConfigCtrl(i2cBusCtrl1, 
                         i2cBusCtrl2,
                         seriallogger, 
                         &quadFlightData,
                         &motorConfig,
                         &quadSystemStatus,
                         &sysCtrlConfig,
                         &dataRequest);
                         
                                                           
    //Setup command message controller
    modeController->SetMode(FlightMode::StartUp);//CommsSynch); //was setup
    BinaryCmdMsgController binaryMsgCmdController(modeController, 
                                                  highLevelRateController,
                                                  &userResponce,
                                                  &dataRequest, 
                                                  &flightCtrlData,
                                                  &systemStatusAck,
                                                  &quadSystemStatus.heartBeatStatus, 
												  &quadSystemStatus.timeSynchStatus,
                                                  seriallogger,
                                                  systemConfig);
                                                  
    binaryMsgCmdController.CmdMsgReader(sysCtrlConfig.systemComms,&BaseComms::TakeDataPacket);
    binaryMsgCmdController.CmdMsgParser(&binaryDataPckParser, &BinaryParser::Parse);
    
    cmdMsgController = &binaryMsgCmdController;
    
    //Setup tasks
    
    //Heartbeatsend task
  //  SchedulerTypes::SchedulerItem HrtBeatSendTask  = {10000,
   // 		0,
	//		SchedulerTypes::HeartBeatSend,
	//		new HeartBeatSendTask(heartBeatMonitor)};

    //Heartbeatmonitor task
        SchedulerTypes::SchedulerItem HrtBeatMonitorTask  = {10000,
        		0,
    			SchedulerTypes::HeartBeatMonitor,
    			new HeartBeatMonitorTask(heartBeatMonitor,&quadSystemStatus.timeSynchStatus)};

    //PWM Test Task
    SchedulerTypes::SchedulerItem PWMOutputTestTask = {1000,
                                             0,
                                             SchedulerTypes::PWMTest, 
                                             new PWMTestTask(sysCtrlConfig.pwmTestCtrl,sysCtrlConfig.pwmTestBusCtrl)};
    
    //Comms Synch
    CommsSynchTask* commsSynch= new CommsSynchTask(sysCtrlConfig.systemComms,modeController );
    SchedulerTypes::SchedulerItem ComsSynchTask = {2000,
                                             0,
                                             SchedulerTypes::CommsSynch, 
                                             commsSynch};
                                             
                                             //Check Comms Receive Queue Task
    SchedulerTypes::SchedulerItem CheckReceiveQueueTask = {5,
                                             0,
                                             SchedulerTypes::CheckReceiveQueue, 
                                             new CheckCommsReceiveQueueTask(sysCtrlConfig.systemComms)
                                             };
                                             
                                             
    //StartUp task
    SystemStartTask* systemStartUpTask = new SystemStartTask(&systemStatusAck);
    
    systemStartUpTask->SendRequest(sysCtrlConfig.systemComms, &BaseComms::SendDataPacket);
    
    SchedulerTypes::SchedulerItem StartUpTask = {300,
                                             0,
                                             SchedulerTypes::StartUp, 
                                             systemStartUpTask};
    
    //Configuration and calibration task
    ConfigureCalibrateTask* systemConfigCalTask = new ConfigureCalibrateTask(&sysCtrlConfig.calibrationCtrl, &sysCtrlConfig.systemConfigCtrl, NULL);
    systemConfigCalTask->SendRequest(sysCtrlConfig.systemComms, &BaseComms::SendDataPacket);
    
    SchedulerTypes::SchedulerItem ConfigCalTask = {1000,
                                             0,
                                             SchedulerTypes::ConfigCal, 
                                             systemConfigCalTask};
    
    //Heartbeat task
    SchedulerTypes::SchedulerItem heartBeatTask = {1000,
                                            0,
                                             SchedulerTypes::ToggleHeartBeat, 
                                             new HeartBeatIndicatorTask(&heartBeat)};                                       
    //Flightdata logger task
    SchedulerTypes::SchedulerItem flightDataTask = {300,
                                             0,
                                             SchedulerTypes::SendFlightData, 
                                             new FlightDataLoggerTask(flightDataEncoder)};
    
    //StatusData logger task
    SchedulerTypes::SchedulerItem StatusDataTask = {10000,
                                             0,
                                             SchedulerTypes::SendSystemStatus, 
                                             new SystemStatusLoggerTask(systemStatusEncoder)};
    //Flightpack monitor task
    SchedulerTypes::SchedulerItem flightPackMonitorTask = {5000,
                                             0,
                                             SchedulerTypes::FlightPackMonitor, 
                                             new FlightPackMonitorTask(sysCtrlConfig.flightPackMonitor,
                                                                      sysCtrlConfig.flightPackCellSelector,
                                                                       sysCtrlConfig.flightPckMntBusCtrl)};                                                                               
    //Altitude task    
    SchedulerTypes::SchedulerItem alitutdeDataTask = {20,
                                             0,
                                             SchedulerTypes::AltimeterUpdate,
                                             new AltimeterTask(sysCtrlConfig.altimeterController,sysCtrlConfig.altimeterBusCtrl)};                                                                       
    //IMU Task    
    SchedulerTypes::SchedulerItem imuTask = {20,
                                             0,
                                             SchedulerTypes::IMUUpdate, 
                                             new IMUTask(sysCtrlConfig.imu,sysCtrlConfig.imuBusCtrl,&imuFlightData)
                                           
                                             
                                           };
                                             
    //Low level Rate controller
    SchedulerTypes::SchedulerItem lowLevelRateCtrl = {10,
                                                      0,
                                                      SchedulerTypes::UpdateLowLevelRateCtrl, 
                                                      new LowLevelFlightControllerTask(lowLevelRateController)
                                                      };
    
    //High level Rate controller
    SchedulerTypes::SchedulerItem highLevelRateCtrl = {20,
                                                      0,
                                                      SchedulerTypes::UpdateHighLevelRateCtrl, 
                                                      new HighLevelFlightControllerTask(highLevelRateController)
                                                      };
                                                                                               
    //Command Reader Task
    SchedulerTypes::SchedulerItem cmdReaderTask = {500,
                                             0,
                                             SchedulerTypes::CheckCmdMsgReceived, 
                                             new CmdReaderTask(cmdMsgController)
                                             };
     //ReSend Last DataPck Task
    SchedulerTypes::SchedulerItem reSendLastDataPckTask = {5000,
                                             0,
                                             SchedulerTypes::ReSendLastDataPck, 
                                             new ReSendLastDataPckTask(sysCtrlConfig.systemComms)
                                             };

    
    //GPS Task
    SchedulerTypes::SchedulerItem gpsTask = {50,
                                             0,
                                             SchedulerTypes::GPSUpdate, 
                                             new GPSTask(sysCtrlConfig.gpsConfigCtrl)};
                                                                                                                               
    //Setup system mode to task mapper
    modeToTaskController = new BasicTaskModeController(&flightSystemMode);
    modeToTaskController->ResetTaskModeMapping();
    ModeToTaskSetup::SetupModeTaskMapping(modeToTaskController);
    
    
    //Setup task scheduler
     taskScheduler = new TimeBasedScheduler(modeToTaskController);
     
    //Always on Tasks
    taskScheduler->AddTask(&ComsSynchTask);
    taskScheduler->AddTask(&CheckReceiveQueueTask);
    taskScheduler->AddTask(&cmdReaderTask);
    //taskScheduler->AddTask(&HrtBeatSendTask);
    taskScheduler->AddTask(&HrtBeatMonitorTask);
   // taskScheduler->AddTask(&flightPackMonitorTask);
   // taskScheduler->AddTask(&flightDataTask);
  //  taskScheduler->AddTask(&StatusDataTask);
    taskScheduler->AddTask(&heartBeatTask);
    taskScheduler->AddTask(&reSendLastDataPckTask);
    
    //Start up Tasks
    taskScheduler->AddTask(&StartUpTask);
    
    //System configuration Tasks
    taskScheduler->AddTask(&ConfigCalTask);
    
    //system configuration complete Tasks
    taskScheduler->AddTask(&imuTask);
    taskScheduler->AddTask(&alitutdeDataTask);
    taskScheduler->AddTask(&gpsTask);
    
    //System ready entering flight mode
    taskScheduler->AddTask(&lowLevelRateCtrl);
    taskScheduler->AddTask(&highLevelRateCtrl);

    //Seed to the rtc.  
    set_time(1256729737);  // Set RTC time to Wed, 28 Oct 2009 11:35:37
    
    //Create system timer in time this will be replaced by the real time clock.
    
    Timer testScheduleTimer;
    
    testScheduleTimer.start();
    seriallogger->EnableDisableLogger(true);
    
    while(1) 
    {
        if (testScheduleTimer.read_us() < 0)
        {
            testScheduleTimer.reset();
            taskScheduler->ResetStartCounters();
        }
        
        taskScheduler->UpdateTaskExecution(testScheduleTimer.read_ms());
    }
}

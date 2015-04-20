/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "UWPoseEstimator.hpp"

using namespace pose_estimation;

UWPoseEstimator::UWPoseEstimator(std::string const& name)
    : UWPoseEstimatorBase(name)
{
}

UWPoseEstimator::UWPoseEstimator(std::string const& name, RTT::ExecutionEngine* engine)
    : UWPoseEstimatorBase(name, engine)
{
}

UWPoseEstimator::~UWPoseEstimator()
{
}

void UWPoseEstimator::depth_samplesTransformerCallback(const base::Time &ts, const ::base::samples::RigidBodyState &depth_samples_sample)
{
    MeasurementConfig config;
    config.measurement_mask[BodyStateMemberZ] = 1;
    config.measurement_mask[BodyStateMemberVz] = 1;
    handleMeasurement(ts, depth_samples_sample, config, _pressure_sensor2body);
}

void UWPoseEstimator::orientation_samplesTransformerCallback(const base::Time &ts, const ::base::samples::RigidBodyState &orientation_samples_sample)
{
    MeasurementConfig config;
    config.measurement_mask[BodyStateMemberRoll] = 1;
    config.measurement_mask[BodyStateMemberPitch] = 1;
    config.measurement_mask[BodyStateMemberYaw] = 1;
    config.measurement_mask[BodyStateMemberVroll] = 1;
    config.measurement_mask[BodyStateMemberVpitch] = 1;
    config.measurement_mask[BodyStateMemberVyaw] = 1;
    handleMeasurement(ts, orientation_samples_sample, config, _imu2body);
}

void UWPoseEstimator::lbl_position_samplesTransformerCallback(const base::Time &ts, const ::base::samples::RigidBodyState &lbl_position_samples_sample)
{
    MeasurementConfig config;
    config.measurement_mask[BodyStateMemberX] = 1;
    config.measurement_mask[BodyStateMemberY] = 1;
    config.measurement_mask[BodyStateMemberZ] = 1;
    handleMeasurement(ts, lbl_position_samples_sample, config, _lbl2body);
}

void UWPoseEstimator::xy_position_samplesTransformerCallback(const base::Time &ts, const ::base::samples::RigidBodyState &xy_position_samples_sample)
{
    MeasurementConfig config;
    config.measurement_mask[BodyStateMemberX] = 1;
    config.measurement_mask[BodyStateMemberY] = 1;
    handleMeasurement(ts, xy_position_samples_sample, config);
}

void UWPoseEstimator::dvl_velocity_samplesTransformerCallback(const base::Time &ts, const ::base::samples::RigidBodyState &dvl_velocity_samples_sample)
{
    MeasurementConfig config;
    config.measurement_mask[BodyStateMemberVx] = 1;
    config.measurement_mask[BodyStateMemberVy] = 1;
    config.measurement_mask[BodyStateMemberVz] = 1;
    handleMeasurement(ts, dvl_velocity_samples_sample, config, _dvl2body);
}

void UWPoseEstimator::model_velocity_samplesTransformerCallback(const base::Time &ts, const ::base::samples::RigidBodyState &model_velocity_samples_sample)
{
    MeasurementConfig config;
    config.measurement_mask[BodyStateMemberVx] = 1;
    config.measurement_mask[BodyStateMemberVy] = 1;
    handleMeasurement(ts, model_velocity_samples_sample, config);
}


void UWPoseEstimator::gps_position_samplesTransformerCallback(const base::Time &ts, const ::base::samples::RigidBodyState &gps_position_samples_sample)
{
  MeasurementConfig config;
  config.measurement_mask[BodyStateMemberX] = 1;
  config.measurement_mask[BodyStateMemberY] = 1;
  
  handleMeasurement(ts, gps_position_samples_sample, config,  _gps2body);
  
}


void UWPoseEstimator::reset_position_samplesTransformerCallback(const base::Time &ts, const ::base::samples::RigidBodyState &reset_position_samples_sample)
{
  base::samples::RigidBodyState rbs;
  rbs.position = reset_position_samples_sample.position;
  rbs.cov_position = base::Matrix3d::Identity() * 1e-9; //Use very small variance 
  
  MeasurementConfig config;
  config.measurement_mask[BodyStateMemberX] = 1;
  config.measurement_mask[BodyStateMemberY] = 1;
  
  //If we have a valid orientation -> Use yaw
  if(reset_position_samples_sample.hasValidOrientation())
  {
    rbs.orientation = reset_position_samples_sample.orientation;
    rbs.cov_position = base::Matrix3d::Identity() * 1e-9;
    
    config.measurement_mask[BodyStateMemberYaw] = 1;    
  }
  
  handleMeasurement(ts, rbs, config);
  
}


void UWPoseEstimator::reset_to_origin()
{
  base::samples::RigidBodyState rbs;
  rbs.position = base::Vector3d::Zero();
  
  reset_position_samplesTransformerCallback(base::Time::now(), rbs);
}

/// The following lines are template definitions for the various state machine
// hooks defined by Orocos::RTT. See UWPoseEstimator.hpp for more detailed
// documentation about them.

bool UWPoseEstimator::configureHook()
{
    if (! UWPoseEstimatorBase::configureHook())
        return false;
    
    source_frame = _body_frame.get();
    
    return true;
}
bool UWPoseEstimator::startHook()
{
    if (! UWPoseEstimatorBase::startHook())
        return false;
    return true;
}
void UWPoseEstimator::updateHook()
{
    UWPoseEstimatorBase::updateHook();
    
    // verify stream aligner status
    verifyStreamAlignerStatus(_transformer);
    
    // update and write new state
    updateState();
}
void UWPoseEstimator::errorHook()
{
    UWPoseEstimatorBase::errorHook();
}
void UWPoseEstimator::stopHook()
{
    UWPoseEstimatorBase::stopHook();
}
void UWPoseEstimator::cleanupHook()
{
    UWPoseEstimatorBase::cleanupHook();
}

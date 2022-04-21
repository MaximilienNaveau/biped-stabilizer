/*
 * Copyright 2021 PAL Robotics SL. All Rights Reserved
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited,
 * unless it was supplied under the terms of a license agreement or
 * nondisclosure agreement with PAL Robotics SL. In this case it may not be
 * copied or disclosed except in accordance with the terms of that agreement.
 */

#ifndef BIPED_STABILIZER_COP_STABILIZER
#define BIPED_STABILIZER_COP_STABILIZER

#include <Eigen/Dense>
#include <vector>

#include "biped_stabilizer/third_party/wykobi/wykobi.hpp"

namespace biped_stabilizer {

typedef wykobi::polygon<double, 2> Polygon2D;
typedef Eigen::Matrix<double, 2, 1> eVector2;
typedef Eigen::Matrix<double, 3, 1> eVector3;
typedef Eigen::Matrix<double, 6, 1> eVector6;
typedef Eigen::Isometry3d eMatrixHom;
typedef Eigen::Matrix3d eMatrixRot;
typedef std::vector<eMatrixHom, Eigen::aligned_allocator<eMatrixHom>>
    eMatrixHoms;
typedef std::vector<eVector2, Eigen::aligned_allocator<eVector2>> eVector2s;
typedef std::vector<eVector3, Eigen::aligned_allocator<eVector3>> eVector3s;

struct CopStabilizerSettings {
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  double height = 0.0;
  double foot_length = 0.0;
  double foot_width = 0.0;
  double robot_mass = 0.0;
  double dt = 0.0;
  eVector3 cop_x_gains = eVector3::Zero();
  eVector3 cop_y_gains = eVector3::Zero();
  double cop_p_cc_gain = 0.0;
  eVector2 integral_gain = eVector2::Zero();
  // Meaningfull defaults.
  double g = 9.81;
  std::string cop_control_type = "p_cc";
  bool saturate_cop = true;
  bool use_rate_limited_dcm = false;

  friend bool operator==(const CopStabilizerSettings &lhs, 
                       const CopStabilizerSettings &rhs){
    bool test = true;
    test &= lhs.height == rhs.height;
    test &= lhs.foot_length == rhs.foot_length;
    test &= lhs.foot_width == rhs.foot_width;
    test &= lhs.robot_mass == rhs.robot_mass;
    test &= lhs.dt == rhs.dt;
    test &= lhs.cop_x_gains == rhs.cop_x_gains;
    test &= lhs.cop_y_gains == rhs.cop_y_gains;
    test &= lhs.cop_p_cc_gain == rhs.cop_p_cc_gain;
    test &= lhs.integral_gain == rhs.integral_gain;
    test &= lhs.g == rhs.g;
    test &= lhs.saturate_cop == rhs.saturate_cop;
    test &= lhs.use_rate_limited_dcm == rhs.use_rate_limited_dcm;
    return test;
  }
};

class CopStabilizer {
 public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  CopStabilizer();

  /**
   * @brief CopStabilizer
   * @param g the gravity constant along z
   * @param height the default height of the CoM
   * @param dt the timestep used by the controller
   * @param cop_gains the gains (x, y, z) used by the feedback law
   */
  CopStabilizer(const CopStabilizerSettings &settings);

  virtual ~CopStabilizer();

  void configure(const CopStabilizerSettings &settings);

  const CopStabilizerSettings &get_settings() { return settings_; }

  void stabilize(const eVector3 &actual_com, 
                 const eVector3 &actual_com_vel,
                 const eVector3 &actual_com_acc, 
                 const eVector3 &actual_cop,
                 const eMatrixHoms &actual_stance_poses,
                 const eVector3 &reference_com,
                 const eVector3 &reference_com_vel,
                 const eVector3 &reference_com_acc,
                 const eVector3 &reference_com_jerk, 
                 eVector3 &desired_com,
                 eVector3 &desired_com_vel, 
                 eVector3 &desired_com_acc,
                 eVector3 &desired_icp,            // ???
                 eVector3 &actual_icp,             // ???
                 eVector3 &desired_cop_reference,  // ???
                 eVector3 &desired_cop_computed);

  void stabilizeCOP(const eVector3 &actual_com, 
                    const eVector3 &actual_com_vel,
                    const eVector3 &actual_com_acc, 
                    const eVector3 &actual_cop,
                    const eMatrixHoms &actual_stance_poses,
                    const eVector3 &reference_com,
                    const eVector3 &reference_com_vel,
                    const eVector3 &reference_com_acc, 
                    eVector3 &desired_com,
                    eVector3 &desired_com_vel, 
                    eVector3 &desired_com_acc,
                    eVector3 &desired_icp,            // ???
                    eVector3 &actual_icp,             // ???
                    eVector3 &desired_cop_reference,  // ???
                    eVector3 &desired_cop_computed);

  void stabilizeApproximateAcceleration(
      const eVector3 &actual_com, 
      const eVector3 &actual_com_vel,
      const eVector3 &actual_com_acc, 
      const eVector3 &actual_cop,
      const eMatrixHoms &actual_stance_poses, 
      const eVector3 &reference_com,
      const eVector3 &reference_com_vel, 
      const eVector3 &reference_com_acc,
      eVector3 &desired_com, 
      eVector3 &desired_com_vel,
      eVector3 &desired_com_acc,
      eVector3 &desired_icp,            // ???
      eVector3 &actual_icp,             // ???
      eVector3 &desired_cop_reference,  // ???
      eVector3 &desired_cop_computed);

  void stabilizeP_CC(const eVector3 &actual_com, 
                     const eVector3 &actual_com_vel,
                     const eVector3 &actual_com_acc, 
                     const eVector3 &actual_cop,
                     const eMatrixHoms &actual_stance_poses,
                     const eVector3 &reference_com,
                     const eVector3 &reference_com_vel,
                     const eVector3 &reference_com_acc, 
                     eVector3 &desired_com,
                     eVector3 &desired_com_vel, 
                     eVector3 &desired_com_acc,
                     eVector3 &desired_icp,            // ???
                     eVector3 &actual_icp,             // ???
                     eVector3 &desired_cop_reference,  // ???
                     eVector3 &desired_cop_computed);

  void stabilizeJerk(const eVector3 &actual_com, 
                     const eVector3 &actual_com_vel,
                     const eVector3 &actual_com_acc, 
                     const eVector3 &actual_cop,
                     const eMatrixHoms &actual_stance_poses,
                     const eVector3 &reference_com,
                     const eVector3 &reference_com_vel,
                     const eVector3 &reference_com_acc,
                     const eVector3 &reference_com_jerk, 
                     eVector3 &desired_com,
                     eVector3 &desired_com_vel, 
                     eVector3 &desired_com_acc,
                     eVector3 &desired_icp,            // ???
                     eVector3 &actual_icp,             // ???
                     eVector3 &desired_cop_reference,  // ???
                     eVector3 &desired_cop_computed);

  std::array<eVector3, 3> getStableCoMs(const std::string &control_type,
                                        const double &com_height);

 private:
  void computeSupportPolygon(const eMatrixHoms &stance_poses,
                             Polygon2D &convex_hull);

  void projectCOPinSupportPolygon(const eVector2 &target_cop_unclamped,
                                  const Polygon2D &polygon,
                                  eVector2 &target_cop);

  bool isPointInPolygon(eVector2 &point, Polygon2D &polygon);

  /**
   * @brief getActualCOM_acc compute COM acceleration from the contact forces
   * measured by the F/T sensors
   * @param bc_
   * @param externalForce is any external force affecting the robot exept
   * gravity and ground contact forces.
   * @return the COM acceleration
   */
  Eigen::Vector3d getActualCOM_acc(const Eigen::Vector3d &externalForce);

  /**
   * @brief movingAverage Insert x at the beginning of the queue, remove all the
   * elements after Nsample from the queue and return the average of all the
   * elements in the queue
   * @param x
   * @param Nsamples
   * @param MA_queue
   * @return
   */
  template <typename T, typename vec_T>
  T movingAverage(const T x, const unsigned long nb_samples, vec_T &queue);

  void getNonLinearPart(const eVector6 &leftFootWrench,
                        const eVector6 &rightFootWrench,
                        const Eigen::Vector2d &leftFootPlace,
                        const Eigen::Vector2d &rightFootPlace,
                        const Eigen::Vector2d &CoM,
                        const Eigen::Vector2d &lateral_gravity,
                        const Eigen::Vector2d &externalForce, 
                        eVector3 &n);

  void getNonLinearPart(const eVector6 &leftFootWrench,
                        const eVector6 &rightFootWrench,
                        const Eigen::Vector2d &leftFootPlace,
                        const Eigen::Vector2d &rightFootPlace,
                        const Eigen::Vector2d &CoM,
                        const Eigen::Vector2d &CoM_acc, 
                        eVector3 &n);

  void getNonLinearPart(const eVector6 &leftFootWrench,
                        const eVector6 &rightFootWrench,
                        const Eigen::Vector2d &leftFootPlace_c,
                        const Eigen::Vector2d &rightFootPlace_c,
                        const Eigen::Vector2d &CoM_acc, 
                        eVector3 &n);

  void getNonLinearPart(const eVector3 &com, 
                        const eVector3 &com_acc,
                        const eVector3 &cop, 
                        eVector3 &n);

  void getNonLinearPart(eVector3 &n);

  bool configured_, first_iter_;
  eMatrixRot A_, Aj_;
  eVector3 B_, Bj_;
  Eigen::Matrix2d A22_;
  eVector2 B2_;
  eVector3 cx_gainK_, cy_gainK_;
  eVector2 cx_gainK2_, cy_gainK2_;
  bool saturate_cop_;

  // sqrt(g/h), g/h
  double w_, w2_;

  Eigen::Matrix3d S_coms_j_;
  Eigen::Matrix<double, 3, 2> S_coms_;
  Eigen::Vector3d U_coms_, U_coms_j_;

  Eigen::Vector3d old_reference_com_acc_;
  eVector3s jerk_ma_queue_;
  Eigen::Vector3d target_com_, target_com_vel_, target_com_acc_,
      target_com_jerk_, non_linear_;
  eVector2 target_cop_, desired_uncampled_cop_;
  eVector2 errorSum_;
  eVector2 cop_clamped;

  // Storing data for stable CoMs computations.
  eVector3 actualState3d_x_, actualState3d_y_;
  eVector2 actualState2d_x_, actualState2d_y_;
  eVector2 actual_command_;

  // Support polygone internal variables.
  eVector3s local_foot_description_;
  eVector3s foot_description_;
  Polygon2D support_polygon_;
  std::vector<wykobi::point2d<double>> wykobi_foot_description_;
  wykobi::point2d<double> wykobi_point2d_;

  // projectCOPinSupportPolygon internal variables.
  wykobi::point2d<double> wykobi_cop_unclamped_;
  wykobi::point2d<double> wykobi_cop_clamped_;

  // isPointInPolygon internal variables.
  wykobi::point2d<double> wykobi_2d_point_;

  // Input settings.
  CopStabilizerSettings settings_;
};
}  // namespace biped_stabilizer
#endif // BIPED_STABILIZER_COP_STABILIZER

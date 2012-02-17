#ifndef QP_GENERATOR
#define QP_GENERATOR

////////////////////////////////////////////////////////////////////////////////
///
///\file	qp-generator.h
///\brief	A class to compute QP elements (objective, constraints, warmstart)
///\author	Herdt Andrei
///\author	Lafaye Jory
///\author	Keith François
///\version	1.0
///\date	05/01/12
///
////////////////////////////////////////////////////////////////////////////////



#include <mpc-walkgen/types.h>

#include <mpc-walkgen/qp-solver.h>
#include <mpc-walkgen/qp-preview.h>

#include <Eigen/Dense>
#include <vector>

namespace MPCWalkgen{

	class QPGenerator{

		public:
			QPGenerator(QPPreview * preview, QPSolver * solver,
					VelReference * velRef, QPPonderation * ponderation,
					RigidBodySystem * robot, const MPCData * generalData);
			~QPGenerator();

			void precomputeObjective();

			void buildObjective(const MPCSolution & result);

			void buildConstraints(const MPCSolution & result);

			void computeWarmStart(MPCSolution & result);

			void convertCopToJerk(MPCSolution & result);

			void computeReferenceVector(const MPCSolution & result);

			void display(const MPCSolution & result, const std::string & filename) const;

		private:

			void buildInequalitiesFeet(const MPCSolution & result);

			void buildConstraintsFeet(const MPCSolution & result);

			void buildConstraintsCOP(const MPCSolution & result);

		private:

		    QPPreview * preview_;
		    QPSolver * solver_;
		    RigidBodySystem * robot_;
		    VelReference * velRef_;
		    QPPonderation * ponderation_;
		    const MPCData * generalData_;

		    Eigen::VectorXd tmpVec_;
		    Eigen::VectorXd tmpVec2_;
		    Eigen::MatrixXd tmpMat_;
		    Eigen::MatrixXd tmpMat2_;

		    RelativeInequalities feetInequalities_;//TODO: Maybe should be instantiated in robot_

		    std::vector<Eigen::MatrixXd> Qconst_;
		    std::vector<Eigen::MatrixXd> choleskyConst_;
		    std::vector<Eigen::MatrixXd> pconstCoM_;
		    std::vector<Eigen::MatrixXd> pconstVc_;
		    std::vector<Eigen::MatrixXd> pconstRef_;

	};

}

/*! \fn MPCWalkgen::QPGenerator::QPGenerator(QPPreview * preview, QPSolver * solver,
					VelReference * velRef, QPPonderation * ponderation,
					RigidBodySystem * robot, int N)
* \brief Constructor
* \param N QP number of samplings
*/

/*! \fn MPCWalkgen::QPGenerator::buildObjective(MPCSolution & result)
* \brief Build matrix Q and vector p for the QP
*/

/*! \fn MPCWalkgen::QPGenerator::buildConstraints(MPCSolution & result)
* \brief build matrix A and vectors BU, BL, XU, XL for the QP
*/

/*! \fn MPCWalkgen::QPGenerator::computeWarmStart(MPCSolution & result)
* \brief Compute a feasible solution and an active-set of previewed constraints
*/

/*! \fn MPCWalkgen::QPGenerator::convertCopToJerk(MPCSolution & result)
* \brief the current QP generate a solution in CoP position. for the interpolation, we must to convert the solution into CoM Jerk
*/

/*! \fn MPCWalkgen::QPGenerator::NbPreviewSamplings(const int N)
* \brief Setter to modify the QP number of samplings
*/

/*! \fn MPCWalkgen::QPGenerator::computeReferenceVector(MPCSolution & result)
* \brief Compute the reference vector (constant over the horizon)
* \param
*/

/*! \fn MPCWalkgen::QPGenerator::display(MPCSolution & result, const std::string & filename)
* \brief display some informations of the solution in a file (previewed CoM, CoP and feet and CoP constraints)
* \param filename the file to display solution
*/

#endif //QP_GENERATOR

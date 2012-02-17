/*
*/



#include <mpc-walkgen/sharedpgtypes.h>
#include <mpc-walkgen/walkgen.h>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <stdio.h>

using namespace Eigen;
using namespace MPCWalkgen;

/*
*/

void makeScilabFile(std::string type);
void dumpTrajectory(MPCSolution & result, std::vector<std::ofstream*> & data_vec);
bool checkFiles(std::ifstream & f1, std::ifstream & f2);
int copyFile(char const * const source, char const * const destination);

int main ()
{

	int nbFile=8;

	std::vector<std::string> name_vec(nbFile);
	name_vec[0]="CoM_X";
	name_vec[1]="CoM_Y";
	name_vec[2]="CoP_X";
	name_vec[3]="CoP_Y";
	name_vec[4]="LF_X";
	name_vec[5]="LF_Y";
	name_vec[6]="RF_X";
	name_vec[7]="RF_Y";

	std::vector<std::ofstream*> data_vec(nbFile);
	std::vector<std::ifstream*> ref_vec(nbFile);
	for(int i=0;i<nbFile;++i){
		data_vec[i] = new std::ofstream((name_vec[i]+".data").c_str());
		ref_vec[i] = new std::ifstream((name_vec[i]+".ref").c_str());
	}


	double robotMass = 0;
	double comHeight = 0.814;

	FootData leftFoot;
	leftFoot.anklePositionInLocalFrame_<< 0, 0, 0.105;
	leftFoot.soleHeight_ = 0.138;
	leftFoot.soleWidth_ = 0.2172;

	FootData rightFoot;
	rightFoot.anklePositionInLocalFrame_ << 0, 0, 	0.105;
	rightFoot.soleHeight_ = 0.138;
	rightFoot.soleWidth_ = 0.2172;

	HipYawData leftHipYaw;
	leftHipYaw.lowerBound_ = -0.523599;
	leftHipYaw.upperBound_ = 0.785398;
	leftHipYaw.lowerVelocityBound_ = -3.54108;
	leftHipYaw.upperVelocityBound_ = 3.54108;
	leftHipYaw.lowerAccelerationBound_ = -0.1;
	leftHipYaw.upperAccelerationBound_ = 0.1;

	HipYawData rightHipYaw = leftHipYaw;

	Walkgen walk(leftFoot, rightFoot,
			leftHipYaw, rightHipYaw,
			robotMass, comHeight, "");

	// initialization
	Eigen::Vector3d leftFootPos(0.00949035, 0.095, 0);
	Eigen::Vector3d rightFootPos(0.00949035, -0.095, 0);
	walk.init(leftFootPos, rightFootPos);

	// run
	double velocity = 0.25;
	walk.reference(0, 0, 0);
	walk.online(0);
	for (double time=0.005;time<5;time+=0.005){
		MPCSolution result = walk.online(time);
		dumpTrajectory(result, data_vec);
	}
	walk.reference(velocity, 0, 0);
	for (double time=5;time<10;time+=0.005){
		MPCSolution result = walk.online(time);
		dumpTrajectory(result, data_vec);
	}
	walk.reference(0, velocity, 0);
	for (double time=10;time<20;time+=0.005){
		MPCSolution result = walk.online(time);
		dumpTrajectory(result, data_vec);
	}
	walk.reference(velocity, 0, velocity);
	for (double time=20;time<30;time+=0.005){
		MPCSolution result = walk.online(time);
		dumpTrajectory(result, data_vec);
	}

	for(unsigned i=0;i<data_vec.size();++i){
		data_vec[i]->close();
	}
	std::vector<std::ifstream*> check_vec(8);
	for(int i=0;i<nbFile;++i){
		check_vec[i] = new std::ifstream((name_vec[i]+".data").c_str());
	}


	for(unsigned i=0;i<check_vec.size();++i){
		if (*ref_vec[i]){
			if (!checkFiles(*check_vec[i],*ref_vec[i])){
				return 1;
			}
		}else{
			copyFile((name_vec[i]+".data").c_str(),(name_vec[i]+".ref").c_str());
		}
		check_vec[i]->close();
		ref_vec[i]->close();
	}
	makeScilabFile("data");
	makeScilabFile("ref");

	return 0;
}

void dumpTrajectory(MPCSolution & result, std::vector<std::ofstream*> & data_vec){
	for(int i=0;i<result.state_vec[0].CoMTrajX_.rows();++i){
		*data_vec[0] << result.state_vec[0].CoMTrajX_(i) << " " << result.state_vec[1].CoMTrajX_(i) << " " << result.state_vec[2].CoMTrajX_(i) << std::endl;
		*data_vec[1] << result.state_vec[0].CoMTrajY_(i) << " " << result.state_vec[1].CoMTrajY_(i) << " " << result.state_vec[2].CoMTrajY_(i) << std::endl;
		*data_vec[2] << result.CoPTrajX(i)  << std::endl;
		*data_vec[3] << result.CoPTrajY(i) << std::endl;
		*data_vec[4] << result.state_vec[0].leftFootTrajX_(i) << " " << result.state_vec[1].leftFootTrajX_(i) << " " << result.state_vec[2].leftFootTrajX_(i) << std::endl;
		*data_vec[5] << result.state_vec[0].leftFootTrajY_(i) << " " << result.state_vec[1].leftFootTrajY_(i) << " " << result.state_vec[2].leftFootTrajY_(i) << std::endl;
		*data_vec[6] << result.state_vec[0].rightFootTrajX_(i) << " " << result.state_vec[1].rightFootTrajX_(i) << " " << result.state_vec[2].rightFootTrajX_(i) << std::endl;
		*data_vec[7] << result.state_vec[0].rightFootTrajY_(i) << " " << result.state_vec[1].rightFootTrajY_(i) << " " << result.state_vec[2].rightFootTrajY_(i) << std::endl;
	}
}

void makeScilabFile(std::string type){

	std::ofstream sci(("plot"+type+".sci").c_str());
	sci << "X=read('CoM_X." << type << "',-1,3);" << std::endl;
	sci << "Y=read('CoM_Y." << type << "',-1,3);" << std::endl;
	sci << "ZX=read('CoP_X." << type << "',-1,1);" << std::endl;
	sci << "ZY=read('CoP_Y." << type << "',-1,1);" << std::endl;
	sci << "LFX=read('LF_X." << type << "',-1,3);" << std::endl;
	sci << "LFY=read('LF_Y." << type << "',-1,3);" << std::endl;
	sci << "RFX=read('RF_X." << type << "',-1,3);" << std::endl;
	sci << "RFY=read('RF_Y." << type << "',-1,3);" << std::endl;
	sci << "s=size(X);t=linspace(0,30,s(1));" << std::endl;

	sci << "subplot(2,2,1);" << std::endl;
	sci << "plot(t,X);" << std::endl;
	sci << "title('CoM_X');" << std::endl;
	sci << "subplot(2,2,3);" << std::endl;
	sci << "plot(t,Y);" << std::endl;
	sci << "title('CoM_Y');" << std::endl;

	sci << "subplot(2,2,2);" << std::endl;
	sci << "plot(t,ZX);" << std::endl;
	sci << "title('CoP_X');" << std::endl;
	sci << "subplot(2,2,4);" << std::endl;
	sci << "plot(t,ZY);" << std::endl;
	sci << "title('CoP_Y');" << std::endl;

	sci << "scf();" << std::endl;

	sci << "plot(X(:,1),Y(:,1),ZX,ZY);" << std::endl;
	sci << "title('CoM_and_CoP_traj');" << std::endl;

	sci << "scf();" << std::endl;

	sci << "subplot(2,2,1);" << std::endl;
	sci << "plot(t,LFX);" << std::endl;
	sci << "title('LF_X');" << std::endl;
	sci << "subplot(2,2,3);" << std::endl;
	sci << "plot(t,LFY);" << std::endl;
	sci << "title('LF_Y');" << std::endl;

	sci << "subplot(2,2,2);" << std::endl;
	sci << "plot(t,RFX);" << std::endl;
	sci << "title('RF_X');" << std::endl;
	sci << "subplot(2,2,4);" << std::endl;
	sci << "plot(t,RFY);" << std::endl;
	sci << "title('RF_Y');" << std::endl;

	sci << "scf();" << std::endl;

	sci << "plot(LFX(:,1),LFY(:,1),RFX(:,1),RFY(:,1));" << std::endl;
	sci << "title('Feet_traj');" << std::endl;

	sci.close();
}

bool checkFiles(std::ifstream & fich1, std::ifstream & fich2){
	bool equal=1;
	if (fich1 && fich2 )
	{
	 std::string lignef1;
	 std::string lignef2;
	 while ( std::getline( fich1, lignef1) && std::getline( fich2, lignef2) && equal)
	 {
	  if (strcmp(lignef1.c_str(),lignef2.c_str())!=0)
	  {
	   equal = 0;
	  }
	 }
	}
	return equal;
}

int copyFile(char const * const source, char const * const destination)
{
    FILE* fSrc;
    FILE* fDest;
    char buffer[512];
    int NbLus;

    if ((fSrc = fopen(source, "rb")) == NULL)
    {
        return 1;
    }

    if ((fDest = fopen(destination, "wb")) == NULL)
    {
        fclose(fSrc);
        return 2;
    }

    while ((NbLus = fread(buffer, 1, 512, fSrc)) != 0)
        fwrite(buffer, 1, NbLus, fDest);

    fclose(fDest);
    fclose(fSrc);

    return 0;
}

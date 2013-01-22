/*--
  Open3DMotion 
  Copyright (c) 2004-2012.
  All rights reserved.
  See LICENSE.txt for more information.
--*/


#include "Open3DMotion/Maths/RigidTransform3.h"

namespace Open3DMotion
{
  void RigidTransform3::FromXYVec(double* R, const double* vx, const double* vxy)
  {
    // x-axis
    R[0] = vx[0];
    R[3] = vx[1];
    R[6] = vx[2];
    
    // normalise
    double d0 = sqrt(R[0]*R[0]+R[3]*R[3]+R[6]*R[6]);
    R[0] /= d0;
    R[3] /= d0;
    R[6] /= d0;

    // orthogonality
    double s = R[0]*vxy[0] + R[3]*vxy[1] + R[6]*vxy[2];

    // Gramm-Schmidt y-axis
    R[1] = vxy[0]-s*R[0];
    R[4] = vxy[1]-s*R[3];
    R[7] = vxy[2]-s*R[6];

    // normalise
    double d2 = sqrt(R[1]*R[1]+R[4]*R[4]+R[7]*R[7]);
    R[1] /= d2;
    R[4] /= d2;
    R[7] /= d2;

    // z-axis
    R[2] = R[3]*R[7]-R[6]*R[4];
    R[5] = R[6]*R[1]-R[0]*R[7];
    R[8] = R[0]*R[4]-R[3]*R[1];
  }

  void RigidTransform3::FromYZVec(double* R, const double* vy, const double* vyz)
  {
    // x-axis
    R[1] = vy[0];
    R[4] = vy[1];
    R[7] = vy[2];
    
    // normalise
    double d0 = sqrt(R[1]*R[1]+R[4]*R[4]+R[7]*R[7]);
    R[1] /= d0;
    R[4] /= d0;
    R[7] /= d0;

    // orthogonality
    double s = R[1]*vyz[0] + R[4]*vyz[1] + R[7]*vyz[2];

    // Gramm-Schmidt z-axis
    R[2] = vyz[0]-s*R[1];
    R[5] = vyz[1]-s*R[4];
    R[8] = vyz[2]-s*R[7];

    // normalise
    double d2 = sqrt(R[2]*R[2]+R[5]*R[5]+R[8]*R[8]);
    R[2] /= d2;
    R[5] /= d2;
    R[8] /= d2;

    // x-axis
    R[0] = R[4]*R[8]-R[7]*R[5];
    R[3] = R[7]*R[2]-R[1]*R[8];
    R[6] = R[1]*R[5]-R[4]*R[2];
  }

  void RigidTransform3::FromZXVec(double* R, const double* vz, const double* vzx)
  {
    // x-axis
    R[2] = vz[0];
    R[5] = vz[1];
    R[8] = vz[2];
    
    // normalise
    double d0 = sqrt(R[2]*R[2]+R[5]*R[5]+R[8]*R[8]);
    R[2] /= d0;
    R[5] /= d0;
    R[8] /= d0;

    // orthogonality
    double s = R[2]*vzx[0] + R[5]*vzx[1] + R[8]*vzx[2];

    // Gramm-Schmidt x-axis
    R[0] = vzx[0]-s*R[2];
    R[3] = vzx[1]-s*R[5];
    R[6] = vzx[2]-s*R[8];

    // normalise
    double d2 = sqrt(R[0]*R[0]+R[3]*R[3]+R[6]*R[6]);
    R[0] /= d2;
    R[3] /= d2;
    R[6] /= d2;

    // z-axis
    R[1] = R[5]*R[6]-R[8]*R[3];
    R[4] = R[8]*R[0]-R[2]*R[6];
    R[7] = R[2]*R[3]-R[5]*R[0];
  }

  void RigidTransform3::FromYXVec(double* R, const double* vy, const double* vyx)
  {
    // y-axis
    R[1] = vy[0];
    R[4] = vy[1];
    R[7] = vy[2];
    
    // normalise
    double d0 = sqrt(R[1]*R[1]+R[4]*R[4]+R[7]*R[7]);
    R[1] /= d0;
    R[4] /= d0;
    R[7] /= d0;

    // orthogonality
    double s = R[1]*vyx[0] + R[4]*vyx[1] + R[7]*vyx[2];

    // Gramm-Schmidt y-axis
    R[0] = vyx[0]-s*R[1];
    R[3] = vyx[1]-s*R[4];
    R[6] = vyx[2]-s*R[7];

    // normalise
    double d2 = sqrt(R[0]*R[0]+R[3]*R[3]+R[6]*R[6]);
    R[0] /= d2;
    R[3] /= d2;
    R[6] /= d2;

    // z-axis
    R[2] = R[3]*R[7]-R[6]*R[4];
    R[5] = R[6]*R[1]-R[0]*R[7];
    R[8] = R[0]*R[4]-R[3]*R[1];
  }

  void RigidTransform3::FromXZVec(double* R, const double* vx, const double* vxz)
  {
    // x-axis
    R[0] = vx[0];
    R[3] = vx[1];
    R[6] = vx[2];
    
    // normalise
    double d0 = sqrt(R[0]*R[0]+R[3]*R[3]+R[6]*R[6]);
    R[0] /= d0;
    R[3] /= d0;
    R[6] /= d0;

    // orthogonality
    double s = R[0]*vxz[0] + R[3]*vxz[1] + R[6]*vxz[2];

    // Gramm-Schmidt z-axis
    R[2] = vxz[0]-s*R[0];
    R[5] = vxz[1]-s*R[3];
    R[8] = vxz[2]-s*R[6];

    // normalise
    double d2 = sqrt(R[2]*R[2]+R[5]*R[5]+R[8]*R[8]);
    R[2] /= d2;
    R[5] /= d2;
    R[8] /= d2;

    // y-axis
    R[1] = R[5]*R[6]-R[8]*R[3];
    R[4] = R[8]*R[0]-R[2]*R[6];
    R[7] = R[2]*R[3]-R[5]*R[0];
  }

  void RigidTransform3::FromZYVec(double* R, const double* vz, const double* vzy)
  {
    // z-axis
    R[2] = vz[0];
    R[5] = vz[1];
    R[8] = vz[2];
    
    // normalise
    double d0 = sqrt(R[2]*R[2]+R[5]*R[5]+R[8]*R[8]);
    R[2] /= d0;
    R[5] /= d0;
    R[8] /= d0;

    // orthogonality
    double s = R[2]*vzy[0] + R[5]*vzy[1] + R[8]*vzy[2];

    // Gramm-Schmidt y-axis
    R[1] = vzy[0]-s*R[2];
    R[4] = vzy[1]-s*R[5];
    R[7] = vzy[2]-s*R[8];

    // normalise
    double d2 = sqrt(R[1]*R[1]+R[4]*R[4]+R[7]*R[7]);
    R[1] /= d2;
    R[4] /= d2;
    R[7] /= d2;

    // x-axis
    R[0] = R[4]*R[8]-R[7]*R[5];
    R[3] = R[7]*R[2]-R[1]*R[8];
    R[6] = R[1]*R[5]-R[4]*R[2];
  }
}

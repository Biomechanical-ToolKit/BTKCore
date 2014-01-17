/*--
  Open3DMotion 
  Copyright (c) 2004-2013.
  All rights reserved.
  See LICENSE.txt for more information.
--*/

#include "Open3DMotion/Biomechanics/Algorithms/Interpolate.h"
#include "Open3DMotion/Biomechanics/Trial/TSFactory.h"
#include "Open3DMotion/Maths/Vector3.h"
#include <limits.h>

namespace Open3DMotion
{
	using namespace std;

	void Interpolate::Linear(TimeSequence& ts)  throw(NoSuchFieldException)
  {
    //size_t numframes = ts.NumFrames(); // Not used
    bool haveseen = false;
		size_t firstvisible(SIZE_MAX);
		size_t lastvisible(SIZE_MAX);
		TSOccVector3Iter iresult(ts);
		TSOccVector3ConstIter isee0(ts);
		TSOccVector3ConstIter isee1(ts);
		for ( ; iresult.HasFrame(); iresult.Next())
    {
			if (!iresult.Occluded())
      {
				if (!haveseen)
				{
					firstvisible = iresult.FrameIndex();
					haveseen = true;
				}
				continue;
      }

      if (!haveseen)
        continue;

      // last seen
			isee0.SeekTo(iresult.FrameIndex()-1);

      // find next visible
      isee1.SeekTo(iresult.FrameIndex()+1);
			while (isee1.HasFrame() && isee1.Occluded())
				isee1.Next();

      // not found
			if (!isee1.HasFrame())
			{
				lastvisible = isee0.FrameIndex();
        break;
			}

      // interpolate to next
			for (; iresult.FrameIndex() < isee1.FrameIndex(); iresult.Next())
      {
				double s = (double)(iresult.FrameIndex()-isee0.FrameIndex()) / (double)(isee1.FrameIndex()-isee0.FrameIndex());
				Vector3::Ratio(iresult.Value(), s, isee0.Value(), isee1.Value());
      }
    }

		//-- fill in start and end with const values

		// fill in start
		if (firstvisible != SIZE_MAX)
		{
			isee0.SeekTo(firstvisible);
			for (iresult.SeekTo(0); iresult.FrameIndex() < firstvisible; iresult.Next())
			{
				memcpy(iresult.Value(), isee0.Value(), 3*sizeof(double));
				iresult.Occluded() = true;
			}
		}

		// fill in end
		if (lastvisible != SIZE_MAX)
		{
			isee0.SeekTo(lastvisible);
			for (iresult.SeekTo(lastvisible+1); iresult.HasFrame(); iresult.Next())
			{
				memcpy(iresult.Value(), isee0.Value(), 3*sizeof(double));
				iresult.Occluded() = true;
			}
		}
  }

	void Interpolate::Cubic(TimeSequence& ts, size_t maxnumslopesamples /*=10*/)  throw(NoSuchFieldException)
  {
    // size_t numframes = ts.NumFrames(); // Not used
    bool haveseen = false;
		size_t firstvisible(SIZE_MAX);
		size_t lastvisible(SIZE_MAX);
		TSOccVector3Iter iresult(ts);
		TSOccVector3ConstIter isee0(ts);
		TSOccVector3ConstIter isee1(ts);
		TSOccVector3ConstIter isee2(ts);
		TSOccVector3ConstIter isee3(ts);
		while (iresult.HasFrame())
    {
			// find non-occluded pair of adjacent frames
			if (!iresult.Occluded())
      {
				iresult.Next();
				if (iresult.HasFrame() && !iresult.Occluded())
				{
					if (!haveseen)
					{
						firstvisible = iresult.FrameIndex() - 1;
						haveseen = true;
					}
					iresult.Next();
					continue;
				}
      }

			// no previous pair of frames - cannot do interp until find them
      if (!haveseen)
			{
				iresult.Next();
        continue;
			}

      // last seen pair
			isee0.SeekTo(iresult.FrameIndex()-2);
			isee1.SeekTo(iresult.FrameIndex()-1);

      // find next visible pair
      isee2.SeekTo(iresult.FrameIndex()+1);
			isee3.SeekTo(iresult.FrameIndex()+2);
			while (isee2.HasFrame() && isee3.HasFrame() && (isee2.Occluded() || isee3.Occluded()))
			{
				isee2.Next();
				isee3.Next();
			}

      // no subsequent pair found
			if (!isee2.HasFrame() || !isee3.HasFrame())
			{
				lastvisible = isee1.FrameIndex();
        break;
			}

			// determine max samples to use either side of gap
			UInt32 gap = isee2.FrameIndex() - isee1.FrameIndex() + 1;
			UInt32 numslopesamples = gap / 2;
			if (numslopesamples < 2)
				numslopesamples = 2;
			else if (numslopesamples > maxnumslopesamples)
				numslopesamples = maxnumslopesamples;

			// go at least one sample left of start of occluded region
			// up to max of numslopesamples whilst still in view
			UInt32 islope;
			for (islope = 1; islope < numslopesamples; islope++)
			{
				isee0.Prev();
				if (!isee0.HasFrame())
					break;
			}
			isee0.Next();

			// next visible after that
			for (islope = 1; islope < numslopesamples; islope++)
			{
				isee3.Next();
				if (!isee3.HasFrame())
					break;
			}
			isee3.Prev();

			// mean value across region before
			UInt32 numbefore = isee1.FrameIndex() - isee0.FrameIndex();
			Vector3 mean_before(0.0);
			for (islope = 0; islope <= numbefore; islope++, isee0.Next())
				mean_before += isee0.Value();
			mean_before /= (numbefore+1);

			// mean value across region afterwards
			UInt32 numafter = isee3.FrameIndex() - isee2.FrameIndex();
			Vector3 mean_after(0.0);
			for (islope = 0; islope <= numafter; islope++, isee3.Prev())
				mean_after += isee3.Value();
			mean_after /= (numafter+1);

			// start and end points
			const Vector3& p0 = isee1.Value();
			const Vector3& p1 = isee2.Value();

			// mean slope before (not really MLE estimate)
			Vector3 m0;
			Vector3::Sub(m0, p0, mean_before);
			m0 /= 0.5 * numbefore;

			// mean slope after (not really MLE estimate)
			Vector3 m1;
			Vector3::Sub(m1, mean_after, p1);
			m1 /= 0.5 * numafter;

      //-- use x, x1, dx0, dx1 to get parameters of x = a + bt + ct2 + dt3:
      //-- (!gap must be (float) or (float)ed to avoid integer overflow of gap*gap*gap)
      double rgap2 = 1.0 / (gap * gap);
      double rgap3 = 1.0 / (gap * gap * gap);

			// compute coeffs of cubic
			Vector3 a, b, c, d;
	    for(UInt32 dim = 0; dim < 3; dim++)
      {
        double p2 = p1[dim] - p0[dim] - (m0[dim] * gap);
        double m2 = m1[dim] - m0[dim];
        a[dim] = p0[dim];
        b[dim] = m0[dim];
        c[dim] = ((3.0F * p2) - (m2 * gap)) * rgap2;
        d[dim] = ((m2 * gap) - (2.0F * p2)) * rgap3;            
      }
            
			// do cubic interp
			for (; iresult.FrameIndex() < isee2.FrameIndex(); iresult.Next())
			{
				double t = (double)(iresult.FrameIndex()-isee1.FrameIndex());
				double* y = iresult.Value();
				for (UInt32 dim = 0; dim < 3; dim++)
					y[dim] = a[dim] + t * (b[dim] + t * (c[dim] + t * d[dim]));
			}
    }

		//-- fill in start and end with const values

		// fill in start
		if (firstvisible != SIZE_MAX)
		{
			isee0.SeekTo(firstvisible);
			for (iresult.SeekTo(0); iresult.FrameIndex() < firstvisible; iresult.Next())
			{
				memcpy(iresult.Value(), isee0.Value(), 3*sizeof(double));
				iresult.Occluded() = true;
			}
		}

		// fill in end
		if (lastvisible != SIZE_MAX)
		{
			isee0.SeekTo(lastvisible);
			for (iresult.SeekTo(lastvisible+1); iresult.HasFrame(); iresult.Next())
			{
				memcpy(iresult.Value(), isee0.Value(), 3*sizeof(double));
				iresult.Occluded() = true;
			}
		}
  }

}

#if 0
		// Archive cubic code from Codamotion Analysis

    //-- cubic interpolation:
    float p0[3] = {x, y, z};
    float p1[3] = {x1, y1, z1};
    float m0[3] = {0.0F, 0.0F, 0.0F};
    float m1[3] = {0.0F, 0.0F, 0.0F};
    float rn;
    int   e;
            
    //-- get slopes either side of gap from average of gap/2 epochs, 
    //-- up to MaxNumSlopeSamples epochs max (more for Pelvis markers)
    int nmax = (int)(gap * 0.5F);
    if(nmax < 2) nmax = 2;
    if(nmax > MaxNumSlopeSamples) nmax = MaxNumSlopeSamples;
            
    //-- get slope at beginning of gap:
    //-- sum points backwards n epochs, if poss:
    float sumX = 0.0F, sumY = 0.0F, sumZ = 0.0F;
    for(int n = 1; n <= nmax; n++)
    {
      e = i0 - n;
      if(e < FirstInViewEpoch )  // || !bMarkerInView[epoch]) - OK already interpolated
        break;
      //-- use already-interpolated data:
      sumX += InterpolatedData[e].Dim[0];
      sumY += InterpolatedData[e].Dim[1];
      sumZ += InterpolatedData[e].Dim[2];              
    }
    if(n > 1)
    {
      rn = 1.0F / (float)(n - 1);
      m0[0] = (x - sumX * rn) * rn * 2.0F;
      m0[1] = (y - sumY * rn) * rn * 2.0F;
      m0[2] = (z - sumZ * rn) * rn * 2.0F;
    }
    //else m0[] = 0
            
    //-- get slope at end of gap:
    //-- sum points forwards n epochs, while still in-view:
    sumX = sumY = sumZ = 0.0F;
    for(n = 1; n <= nmax; n++)
    {
      int e = epoch + n;
      if(e >= NumEpochs || !bMarkerInView[e])
        break;
      sumX += FilteredData[e].Dim[0];
      sumY += FilteredData[e].Dim[1];
      sumZ += FilteredData[e].Dim[2];              
    }
    if(n > 1)
    {
      rn = 1.0F / (float)(n - 1);
      m1[0] = (sumX * rn - x1) * rn * 2.0F;
      m1[1] = (sumY * rn - y1) * rn * 2.0F;
      m1[2] = (sumZ * rn - z1) * rn * 2.0F;
    }
    //else m1[] = 0
            
    //-- use x, x1, dx0, dx1 to get parameters of x = a + bt + ct2 + dt3:
    //-- (!gap must be (float) or (float)ed to avoid integer overflow of gap*gap*gap)
    float rgap3 = 1.0F / (gap * gap * gap);
    float rgap2 = rgap3 * gap;               //yuk? (but avoids a second division)

    //-- for each of X, Y, Z:
    for(int dim = 0; dim < 3; dim++)
    {
      float p2 = p1[dim] - p0[dim] - (m0[dim] * gap);
      float m2 = m1[dim] - m0[dim];
      float a = p0[dim];
      float b = m0[dim];
      float c = ((3.0F * p2) - (m2 * gap)) * rgap2;
      float d = ((m2 * gap) - (2.0F * p2)) * rgap3;
            
      //-- fill-in the gap:
      int imax = epoch - i0;
      for(i = 1; i < imax; i++)
      { 
        //-- (!must (float) i to avoid integer overflow)
        register float t = (float)i;                 
        InterpolatedData[i0 + i].Dim[dim] = a + t * (b + t * (c + t * d));
      }              
    }
  }
#endif
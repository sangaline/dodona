#include  "InputModels/Interpolation.h"

#include "math.h"

//Helper functions
namespace {
    //This function simply returns the list of points from a quadratic bezier interpolation
    //of three control points.
    InputVector QuadraticBezierInterpolation(InputVector& iv, unsigned int Nsteps) {
        InputVector newIV;

        for(unsigned int i = 0; i <= Nsteps; i++)
        {
            //find the endpoints for the line that contains the new points (as a function of t)
            double px1 = iv.X(0)+(iv.X(1)-iv.X(0))*(i/double(Nsteps));
            double py1 = iv.Y(0)+(iv.Y(1)-iv.Y(0))*(i/double(Nsteps));
            double px2 = iv.X(1)+(iv.X(2)-iv.X(1))*(i/double(Nsteps));
            double py2 = iv.Y(1)+(iv.Y(2)-iv.Y(1))*(i/double(Nsteps));

            double newT = iv.T(0)+(iv.T(2)-iv.T(0))*(i/double(Nsteps));

            newIV.AddPoint(px1+(px2-px1)*(i/double(Nsteps)),py1+(py2-py1)*(i/double(Nsteps)),newT);
        }

        return newIV;
    }


    //Helper function that combines a vector of InputVectors into a single continuous InputVector.
    //The input vectors must be ordered correctly in the vector
    InputVector CombineInputVectors(std::vector<InputVector> IVvect) {
        InputVector newIV;

        for(unsigned int i = 0; i < IVvect.size(); i++) {
            if(i==0) {
                for(unsigned int j=0; j < IVvect[i].Length(); j++) {
                    newIV.AddPoint(IVvect[i].X(j),IVvect[i].Y(j),IVvect[i].T(j));
                }
            }

            else {
                for(unsigned int j=1; j < IVvect[i].Length(); j++) {
                    newIV.AddPoint(IVvect[i].X(j),IVvect[i].Y(j),IVvect[i].T(j));
                }
            }
        }

        return newIV;
    }


    //Helper function that returns the distance between point i in input vector iv and the next point in iv
    double DistanceToNextPoint(InputVector& iv, unsigned int i) {
        return sqrt(pow(iv.X(i+1)-iv.X(i),2) + pow(iv.Y(i+1)-iv.Y(i),2));
    }
}

InputVector SpatialInterpolation(InputVector& iv, unsigned int Nsteps) {
    const double length = iv.SpatialLength();
    const unsigned int points = iv.Length();
    const double steplength = length/(double(Nsteps)-1.0);

    InputVector newiv;
    newiv.AddPoint(iv.X(0), iv.Y(0), iv.T(0));
    for(unsigned int i = 1; i < Nsteps-1; i++) {
        double current_distance = steplength*double(i);

        unsigned int low_point = 0, high_point = 1;
        double low_distance = 0, high_distance = 0;

        while(true) {
            high_distance += sqrt( pow( iv.X(high_point)-iv.X(low_point), 2) + pow( iv.Y(high_point) - iv.Y(low_point), 2) );

            if(high_distance >= current_distance || high_point+1 == points) {
                break;
            }

            low_distance = high_distance;
            low_point = high_point;
            high_point++;
        }
        double high_weight = (current_distance - low_distance)/(high_distance-low_distance);
        if(high_distance == low_distance) { high_weight = 0.5; }
        double low_weight = 1.0 - high_weight;

        double newx = iv.X(high_point)*high_weight + iv.X(low_point)*low_weight;
        double newy = iv.Y(high_point)*high_weight + iv.Y(low_point)*low_weight;
        double newt = iv.T(high_point)*high_weight + iv.T(low_point)*low_weight;
        newiv.AddPoint(newx, newy, newt);
    }

    if(Nsteps > 1) {
        newiv.AddPoint(iv.X(points-1), iv.Y(points-1), iv.T(points-1));
    }

    return newiv;
}


//Cubic spline interpolation
InputVector CubicSplineInterpolation(InputVector& iv, unsigned int Nsteps) {
    const unsigned int nPoints = iv.Length();
    const unsigned int nSplines = nPoints-1;

    //no fancy interpolation necessary for one or two letter words
    if (nPoints <= 2)
        return SpatialInterpolation(iv,Nsteps);

    //spline coefficiencts
    double ax[nSplines],bx[nSplines],cx[nSplines],dx[nSplines],alphax[nSplines],zx[nSplines];
    double ay[nSplines],by[nSplines],cy[nSplines],dy[nSplines],alphay[nSplines],zy[nSplines];
    cx[0]=0; cy[0]=0;

    double tstep[nSplines],l[nSplines], mu[nSplines];
    l[0]=1; mu[0]=0; zx[0]=0; zy[0]=0;

    //calculate the spline coefficients
    for(unsigned int i = 0; i < nSplines; i++) {
        ax[i] = iv.X(i);
        ay[i] = iv.Y(i);
        tstep[i] = iv.T(i+1)-iv.T(i);
    }
    ax[nSplines]=iv.X(nSplines); ay[nSplines]=iv.Y(nSplines);

    for(unsigned int i = 1; i < nSplines; i++) {
        alphax[i] = 3*(ax[i+1]-ax[i])/tstep[i] - 3*(ax[i]-ax[i-1])/tstep[i-1];
        alphay[i] = 3*(ay[i+1]-ay[i])/tstep[i] - 3*(ay[i]-ay[i-1])/tstep[i-1];

        l[i] = 2*(tstep[i]+tstep[i-1])-tstep[i-1]*mu[i-1];
        mu[i]= tstep[i]/l[i];

        zx[i] = (alphax[i]-tstep[i-1]*zx[i-1])/l[i];
        zy[i] = (alphay[i]-tstep[i-1]*zy[i-1])/l[i];
    }

    l[nSplines]=1; zx[nSplines]=0; zy[nSplines]=0; cx[nSplines]=0; cy[nSplines]=0;
//    l[nSplines-1]=1; zx[nSplines-1]=0; zy[nSplines-1]=0; cx[nSplines]=0; cy[nSplines]=0;

    for(int i = nSplines-1; i >= 0; i--) {
        if(i != 0) {
            cx[i] = zx[i] - mu[i]*cx[i+1];
            cy[i] = zy[i] - mu[i]*cy[i+1];
        }

        bx[i] = (ax[i+1]-ax[i])/tstep[i] - tstep[i]*(cx[i+1]+2*cx[i])/3;
        by[i] = (ay[i+1]-ay[i])/tstep[i] - tstep[i]*(cy[i+1]+2*cy[i])/3;

        dx[i] = (cx[i+1]-cx[i])/3/tstep[i];
        dy[i] = (cy[i+1]-cy[i])/3/tstep[i];
    }

    //create new input vector to fill with points from the various spline functions
    InputVector newIV;
    
    //walk through each spline function to add to the new input vector
    for(unsigned int i = 0; i < nSplines; i++) {
        double newx, newy, newt;
        double step; 
        unsigned int nSteps = int(Nsteps*DistanceToNextPoint(iv,i)/iv.SpatialLength());

        for(unsigned int j = 0; j < nSteps; j++) {
            if(nSteps == 1) step = 1;
            else step = double(j)/double(nSteps);

            newx = ax[i] + bx[i]*step + cx[i]*pow(step,2) + dx[i]*pow(step,3);
            newy = ay[i] + by[i]*step + cy[i]*pow(step,2) + dy[i]*pow(step,3);
            newt = iv.T(i) + tstep[i]*step;

            newIV.AddPoint(newx,newy,newt);
        }
    }
    newIV.AddPoint(iv.X(nSplines),iv.Y(nSplines),iv.T(nSplines));

    return newIV;
}


//New cubic spline interpolation function
InputVector CubicSplineInterpolationV2(InputVector& iv, unsigned int Nsteps) {
    const unsigned int nPoints = iv.Length();
    const unsigned int nSplines = nPoints-1;

    if (nPoints <= 2)
        return SpatialInterpolation(iv,Nsteps);

    //algorithm coefficients
    double Dx[nPoints],cpx[nSplines],dpx[nPoints];
    double Dy[nPoints],cpy[nSplines],dpy[nPoints];

    //first step of tridiagonal matrix algorithm, a forward step to modify the coefficients
    for(unsigned int i=0; i < nPoints; i++) {
        if(i==0) {
            cpx[i] = 0.5;
            cpy[i] = 0.5;

            dpx[i] = 0.5*3.0*(iv.X(i+1)-iv.X(i));
            dpy[i] = 0.5*3.0*(iv.Y(i+1)-iv.Y(i));
        }
        else if (i < nPoints-1) {
            cpx[i] = 1.0/(4.0-cpx[i-1]);
            cpy[i] = 1.0/(4.0-cpy[i-1]);

            dpx[i] = (3.0*(iv.X(i+1)-iv.X(i-1))-dpx[i-1])/(4.0-cpx[i-1]);
            dpy[i] = (3.0*(iv.Y(i+1)-iv.Y(i-1))-dpy[i-1])/(4.0-cpy[i-1]); 
        }
        else {
            dpx[i] = (3.0*(iv.X(i)-iv.X(i-1))-dpx[i-1])/(2.0-cpx[i-1]);
            dpy[i] = (3.0*(iv.Y(i)-iv.Y(i-1))-dpy[i-1])/(2.0-cpy[i-1]); 
        }
    }

    //Backward substitution step of tridiagnoal matrix algorithm.
    //This obtains the value of the derivative of the interpolation curve at each point.
    if(nPoints > 0) {
        unsigned int i = nPoints - 1;
        Dx[i] = dpx[i];
        Dy[i] = dpy[i];
        while(i > 0) {
             Dx[i-1] = dpx[i-1]-cpx[i-1]*Dx[i];
             Dy[i-1] = dpy[i-1]-cpy[i-1]*Dy[i];
             i--;
         }
    }

    //create new InputVector by walking through each spline curve
    InputVector newIV;
    for(unsigned int i=0; i < nSplines; i++) {
        double newX, newY, newT, step;
        unsigned int nSteps = int(Nsteps*DistanceToNextPoint(iv,i)/iv.SpatialLength());

        for(unsigned int j=0; j < nSteps; j++) {
            if(nSteps==1) step = 1;
            else step = double(j)/double(nSteps);

            newX = iv.X(i) + Dx[i]*step + (3*(iv.X(i+1)-iv.X(i))-2*Dx[i]-Dx[i+1])*pow(step,2) + (2*(iv.X(i)-iv.X(i+1))+Dx[i]+Dx[i+1])*pow(step,3);
            newY = iv.Y(i) + Dy[i]*step + (3*(iv.Y(i+1)-iv.Y(i))-2*Dy[i]-Dy[i+1])*pow(step,2) + (2*(iv.Y(i)-iv.Y(i+1))+Dy[i]+Dy[i+1])*pow(step,3);            
            newT = iv.T(i)+(iv.T(i+1)-iv.T(i))*step;

            if(i==0) {
                newX = iv.X(i) - Dx[i]*step + (2*(iv.X(i)-iv.X(i+1))+Dx[i]+Dx[i+1])*pow(step,3);
                newY = iv.Y(i) - Dy[i]*step + (2*(iv.Y(i)-iv.Y(i+1))+Dy[i]+Dy[i+1])*pow(step,3);
            }
            newIV.AddPoint(newX,newY,newT);
        }
    }
    newIV.AddPoint(iv.X(nSplines),iv.Y(nSplines),iv.T(nSplines));
    return newIV;
}


//Quadratic bezier interpollation (version 2)
InputVector BezierInterpolation(InputVector& iv, unsigned int Nsteps) {
    const unsigned int nPoints = iv.Length();
    const unsigned int nBezPoints = 3*nPoints - 4;

    //no fancy interpolation necessary for one or two letter words
    if (nPoints <= 2)
        return SpatialInterpolation(iv,Nsteps);

    //create a new input vector with the bezier control points included
    InputVector bezIV;

    bezIV.AddPoint(iv.X(0),iv.Y(0),iv.T(0));
    for(unsigned int i = 1; i < nPoints-1; i++) {
        bezIV.AddPoint(iv.X(i)-(iv.X(i)-iv.X(i-1))/4,iv.Y(i)-(iv.Y(i)-iv.Y(i-1))/4,iv.T(i)-(iv.T(i)-iv.T(i-1))/4);
        bezIV.AddPoint(iv.X(i),iv.Y(i),iv.T(i));
        bezIV.AddPoint(iv.X(i)+(iv.X(i+1)-iv.X(i))/4,iv.Y(i)+(iv.Y(i+1)-iv.Y(i))/4,iv.T(i)+(iv.T(i+1)-iv.T(i))/4);
    }
    bezIV.AddPoint(iv.X(nPoints-1),iv.Y(nPoints-1),iv.T(nPoints-1));

    //Create a list of input vectors corresponding to the different segments of the swype pattern.
    //These will all be combined in end to form the final swype pattern
    std::vector<InputVector> IVlist;

    //the first segmens is just a straight line
    InputVector firstSeg;
    firstSeg.AddPoint(bezIV.X(0), bezIV.Y(0), bezIV.T(0));
    firstSeg.AddPoint(bezIV.X(1), bezIV.Y(1), bezIV.T(1));
    IVlist.push_back(firstSeg);

    //create and add the quadratic bezier interpolated input vectors that form the middle section of the swype pattern
    for(unsigned int i = 2; i < nBezPoints; i+=3) {
        //Create the next input vector segment to be bezier interpolated
        InputVector bezSeg;
        bezSeg.AddPoint(bezIV.X(i-1),bezIV.Y(i-1),bezIV.T(i-1));
        bezSeg.AddPoint(bezIV.X(i),bezIV.Y(i),bezIV.T(i));
        bezSeg.AddPoint(bezIV.X(i+1),bezIV.Y(i+1),bezIV.T(i+1));

        int nSegSteps = int(Nsteps*((1.5*DistanceToNextPoint(bezIV,i-1))/bezIV.SpatialLength()));
        IVlist.push_back(QuadraticBezierInterpolation(bezSeg,nSegSteps));

        //Create the next linear segment that connects neighboring bezier segments
        InputVector linSeg;
        linSeg.AddPoint(bezIV.X(i+1),bezIV.Y(i+1),bezIV.T(i+1));
        linSeg.AddPoint(bezIV.X(i+2),bezIV.Y(i+2),bezIV.T(i+2));
        IVlist.push_back(linSeg);
    }

    //combine all of the segments into one input vector
    InputVector combinedIV;
    combinedIV = CombineInputVectors(IVlist);
    return combinedIV;
}


//Very similar to the quadratic bezier interpolation algorith above.  The only difference is that
//the control points here are halfway between each letter.
InputVector BezierSloppyInterpolation(InputVector& iv, unsigned int Nsteps) {
    const unsigned int nPoints = iv.Length();
    const unsigned int nBezPoints = 2*nPoints - 1;

    //no fancy interpolation necessary for one or two letter words
    if (nPoints <= 2)
        return SpatialInterpolation(iv,Nsteps);

    //create a new input vector with the bezier control points included
    InputVector bezIV;

    bezIV.AddPoint(iv.X(0),iv.Y(0),iv.T(0));
    for(unsigned int i = 1; i < nPoints; i++) {
        bezIV.AddPoint((iv.X(i)+iv.X(i-1))/2.0,(iv.Y(i)+iv.Y(i-1))/2.0,(iv.T(i)+iv.T(i-1))/2.0);
        bezIV.AddPoint(iv.X(i),iv.Y(i),iv.T(i));
    }

    //Create a list to hold the input veectors corresponding to various segments of the swype pattern
    std::vector<InputVector> IVlist;

    //the first segmens is just a straight line
    InputVector firstSeg;
    firstSeg.AddPoint(bezIV.X(0), bezIV.Y(0), bezIV.T(0));
    firstSeg.AddPoint(bezIV.X(1), bezIV.Y(1), bezIV.T(1));
    IVlist.push_back(firstSeg);

    //create and add the quadratic bezier interpolated input vectors that form the middle section of the swype pattern
    for(unsigned int i = 2; i < nBezPoints-1; i+=2) {
        //Create the next input vector segment to be bezier interpolated
        InputVector bezSeg;
        bezSeg.AddPoint(bezIV.X(i-1),bezIV.Y(i-1),bezIV.T(i-1));
        bezSeg.AddPoint(bezIV.X(i),bezIV.Y(i),bezIV.T(i));
        bezSeg.AddPoint(bezIV.X(i+1),bezIV.Y(i+1),bezIV.T(i+1));

        int nSegSteps = int(Nsteps*((1.5*DistanceToNextPoint(bezIV,i-1))/bezIV.SpatialLength()));
        IVlist.push_back(QuadraticBezierInterpolation(bezSeg,nSegSteps));
    }

    //the final segment is also just a straight line
    InputVector lastSeg;
    lastSeg.AddPoint(bezIV.X(bezIV.Length()-2),bezIV.Y(bezIV.Length()-2),bezIV.T(bezIV.Length()-2));
    lastSeg.AddPoint(bezIV.X(bezIV.Length()-1),bezIV.Y(bezIV.Length()-1),bezIV.T(bezIV.Length()-1));
    IVlist.push_back(lastSeg);

    //combine all of the segments into a single input vector
    InputVector combinedIV;
    combinedIV = CombineInputVectors(IVlist);
    return combinedIV;
}



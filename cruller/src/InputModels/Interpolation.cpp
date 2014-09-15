#include  "InputModels/Interpolation.h"

#include "math.h"

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

// Basic Quadratic Interpolation
InputVector CubicSplineInterpolation(InputVector& iv, unsigned int Nsteps) {
    const double length = iv.SpatialLength();

    const unsigned int points = iv.Length();
    int n = iv.Length() - 1;

    double length_left = length;
    int Nsteps_left = Nsteps;

    double dist;

    if (points <= 2) return SpatialInterpolation(iv, Nsteps);
    // we add the last point twice, and then don't plot the last piece of the curve
    //int size = iv.Length();
    //iv.AddPoint(iv.X(size-1), iv.Y(size-1), iv.T(size-1));

    InputVector newiv;

    double a_x[n+1],alpha_x[n+1],b_x[n+1],d_x[n+1]; //keep track of spline coefficients in x
    double a_y[n+1],alpha_y[n+1],b_y[n+1],d_y[n+1]; //keep track of spline coefficients in y

    double h[n+1];
    double t[n+1];

    for(int i = 0; i < n+1; ++i) {
        a_x[i] = iv.X(i);
        a_y[i] = iv.Y(i);

        h[i] = iv.T(i+1)-iv.T(i); //corresponds to difference in times.
        t[i] = iv.T(i);
    }

    for(int i = 0; i < n; ++i) {
        alpha_x[i] = 3*(a_x[i+1]-a_x[i])/h[i] - 3*(a_x[i]-a_x[i-1])/h[i-1];
        alpha_y[i] = 3*(a_y[i+1]-a_y[i])/h[i] - 3*(a_y[i]-a_y[i-1])/h[i-1];
    }

    double c_x[n+1],z_x[n+1];
    double c_y[n+1],z_y[n+1];

    double l[n+1],mu[n+1];

    l[0] = 1;
    mu[0] = 0;

    z_x[0] = 0;
    z_y[0] = 0;

    for(int i = 1; i < n; ++i)
    {
        l[i] = 2 *(t[i+1]-t[i-1])-h[i-1]*mu[i-1];
        mu[i] = h[i]/l[i];

        z_x[i] = (alpha_x[i]-h[i-1]*z_x[i-1])/l[i];
        z_y[i] = (alpha_y[i]-h[i-1]*z_y[i-1])/l[i];
    }

    l[n] = 1;
    z_x[n] = 0;
    z_y[n] = 0;

    c_x[n] = 0;
    c_y[n] = 0;

    for(int j = n-1; j >= 0; --j)
    {
        c_x[j] = z_x[j] - mu[j] * c_x[j+1];
        c_y[j] = z_y[j] - mu[j] * c_y[j+1];

        b_x[j] = (a_x[j+1]-a_x[j])/h[j]-h[j]*(c_x[j+1]+2*c_x[j])/3;
        b_y[j] = (a_y[j+1]-a_y[j])/h[j]-h[j]*(c_y[j+1]+2*c_y[j])/3;

        d_x[j] = (c_x[j+1]-c_x[j])/3/h[j];
        d_y[j] = (c_y[j+1]-c_y[j])/3/h[j];
    }

    int time_points;
    double current_time;
    double newx, newy, newt;

    // now, we are going to walk through each of the curves
    for (unsigned int i = 0; i < points-1; i++) {
        dist = sqrt(pow(a_x[i]-a_x[i+1],2)+pow(a_y[i]-a_y[i+1],2));
        // begin by adding the first line
        time_points = Nsteps_left*dist/length_left;

        Nsteps_left = Nsteps_left - time_points;
        length_left = length_left - dist;

        if (i == points-2) time_points = Nsteps-newiv.Length();

        // each curve goes from a time of 0, to a time of h(i)
        for(int time = 0; time < time_points; time++) {
            if (time_points != 1) current_time = double(time)/double(time_points-1);
            else current_time = 1;

            newx = a_x[i] + b_x[i] * current_time + c_x[i] * pow(current_time,2) + d_x[i] * pow(current_time,3);
            newy = a_y[i] + b_y[i] * current_time + c_y[i] * pow(current_time,2) + d_y[i] * pow(current_time,3);
            newt = (1-current_time)*iv.T(i) + current_time*iv.T(i+1);

            newiv.AddPoint(newx, newy, newt);
        }
    }

    return newiv;
}

// Basic Quadratic Interpolation Shifted
InputVector BezierInterpolation(InputVector& iv, unsigned int Nsteps) {
    const double length = iv.SpatialLength();
    const unsigned int points = iv.Length();

    if (points <= 2) return SpatialInterpolation(iv, Nsteps);

    double length_left = length;
    int Nsteps_left = Nsteps;

    double dist;

    InputVector tempiv;
    InputVector newiv;

    // adding points in between. if we instead want to put these turning points somewhere else, scale besides /2
    double x1,y1,t1,x2,y2,t2,x3,y3,t3;

    x2 = iv.X(0);
    y2 = iv.Y(0);
    t2 = iv.T(0);

    tempiv.AddPoint(x2, y2, t2);

    for (unsigned int i = 1; i < points; i++) {
        x1 = x2;
        y1 = y2;
        t1 = t2;

        x2 = iv.X(i);
        y2 = iv.Y(i);
        t2 = iv.T(i);

        if (i!=1) tempiv.AddPoint((3*x1+x2)/4,(3*y1+y2)/4,(3*t1+t2)/4);
        if (i!=points-1) tempiv.AddPoint((x1+3*x2)/4,(y1+3*y2)/4,(t1+3*t2)/4);

        tempiv.AddPoint(x2,y2,t2);
    }

    // we will draw line from 0->1, and then curve from 1->3, 3->5, 5->7, then a line from (n-1)->n
    // we will thus be splitting up into 2 straight lines, and (n-2)/3 curves.
    // for X points -> Y curves; 5 -> 2, 7 -> 3, 9 -> 4. So, split 100 by (n-1)/2

    int time_points;

    double current_time = 0;
    double newx,newy,newt;

    // now, we are going to walk through each of the curves
    for (unsigned int i = 0; i < points-1; i++) {
        // begin by adding the first line
        x1 = tempiv.X(3*i);
        y1 = tempiv.Y(3*i);
        t1 = tempiv.T(3*i);

        x2 = tempiv.X(3*i+1);
        y2 = tempiv.Y(3*i+1);
        t2 = tempiv.T(3*i+1);

        // last part
        if (i == points-2) {
            // time_points = Nsteps*sqrt(pow(x1-x2,2)+pow(y1-y2,2))/length;
            time_points = Nsteps-newiv.Length();

            for(int t = 0; t < time_points; t++) {
                if (time_points != 1) current_time = double(t)/double(time_points-1);
                else current_time = 1;

                newx = x1*(1-current_time) + x2*current_time;
                newy = y1*(1-current_time) + y2*current_time;
                newt = t1*(1-current_time) + t2*current_time;

                newiv.AddPoint(newx, newy, newt);
            }
        } else {
            dist = sqrt(pow(x1-x2,2)+pow(y1-y2,2));
            // begin by adding the first line
            time_points = Nsteps_left*dist/length_left;

            length_left = length_left - dist;
            Nsteps_left = Nsteps_left - time_points;

            for(int t = 0; t < time_points; t++) {
                if (time_points != 1) current_time = double(t)/double(time_points-1);
                else current_time = 1;

                newx = x1*(1-current_time) + x2*current_time;
                newy = y1*(1-current_time) + y2*current_time;
                newt = t1*(1-current_time) + t2*current_time;

                newiv.AddPoint(newx, newy, newt);
            }

            x1 = x2;
            y1 = y2;
            t1 = t2;

            x2 = tempiv.X(3*i+2);
            y2 = tempiv.Y(3*i+2);
            t2 = tempiv.T(3*i+2);

            x3 = tempiv.X(3*i+3);
            y3 = tempiv.Y(3*i+3);
            t3 = tempiv.T(3*i+3);

            dist = sqrt(pow(x1-x2,2)+pow(y1-y2,2))+sqrt(pow(x2-x3,2)+pow(y2-y3,2));
            // begin by adding the first line
            time_points = Nsteps_left*dist/length_left;

            length_left = length_left - dist;
            Nsteps_left = Nsteps_left - time_points;

            for(unsigned int t = 0; t < time_points; t++) {
                if (time_points != 1) current_time = double(t)/double(time_points-1);
                else current_time = 1;

                newx = x1*pow(1-current_time,2) + 2*x2*(1-current_time)*current_time + x3*pow(current_time,2);
                newy = y1*pow(1-current_time,2) + 2*y2*(1-current_time)*current_time + y3*pow(current_time,2);
                newt = t1*(1-current_time) + t3*current_time;

                newiv.AddPoint(newx, newy, newt);
            }
        }
    }

    return SpatialInterpolation(newiv, Nsteps);
}

// Basic Quadratic Interpolation
InputVector BezierSloppyInterpolation(InputVector& iv, unsigned int Nsteps) {
    const double length = iv.SpatialLength();
    const unsigned int points = iv.Length();

    if (points <= 2) return SpatialInterpolation(iv, Nsteps);

    double length_left = length;
    int Nsteps_left = Nsteps;

    double dist;

    InputVector tempiv;
    InputVector newiv;

    // adding points in between. if we instead want to put these turning points somewhere else, scale besides /2
    double x1,y1,t1,x2,y2,t2,x3,y3,t3;

    x2 = iv.X(0);
    y2 = iv.Y(0);
    t2 = iv.T(0);

    tempiv.AddPoint(x2, y2, t2);

    for (unsigned int i = 1; i < points; i++) {
        x1 = x2;
        y1 = y2;
        t1 = t2;

        x2 = iv.X(i);
        y2 = iv.Y(i);
        t2 = iv.T(i);

        tempiv.AddPoint((x1+x2)/2,(y1+y2)/2,(t1+t2)/2);
        tempiv.AddPoint(x2,y2,t2);
    }

    int temp_length = tempiv.Length();

    // we will draw line from 0->1, and then curve from 1->3, 3->5, 5->7, then (n-1)->n
    // we will thus be splitting up into 2 straight lines, and (n-3)/2.
    // for X points -> Y curves; 5 -> 2, 7 -> 3, 9 -> 4. So, split 100 by (n-1)/2

    int time_points;

    double current_time = 0;
    double newx,newy,newt;

    // begin by adding the first line
    x1 = tempiv.X(0);
    y1 = tempiv.Y(0);
    t1 = tempiv.T(0);

    x2 = tempiv.X(1);
    y2 = tempiv.Y(1);
    t2 = tempiv.T(1);

    dist = sqrt(pow(x1-x2,2)+pow(y1-y2,2));
    // begin by adding the first line
    time_points = Nsteps_left*dist/length_left;

    length_left = length_left - dist;
    Nsteps_left = Nsteps_left - time_points;

    for(int t = 0; t < time_points; t++) {
        if (time_points != 1) current_time = double(t)/double(time_points-1);
        else current_time = 1;

        newx = x1*(1-current_time) + x2*current_time;
        newy = y1*(1-current_time) + y2*current_time;
        newt = t1*(1-current_time) + t2*current_time;

        newiv.AddPoint(newx, newy, newt);
    }

    // now, we are going to walk through each of the curves
    for (unsigned int i = 1; i < points-1; i++) {
        x1 = tempiv.X(2*i-1);
        y1 = tempiv.Y(2*i-1);
        t1 = tempiv.T(2*i-1);

        x2 = tempiv.X(2*i);
        y2 = tempiv.Y(2*i);
        t2 = tempiv.T(2*i);

        x3 = tempiv.X(2*i+1);
        y3 = tempiv.Y(2*i+1);
        t3 = tempiv.T(2*i+1);

        dist = sqrt(pow(x1-x2,2)+pow(y1-y2,2))+sqrt(pow(x2-x3,2)+pow(y2-y3,2));
        // begin by adding the first line

        time_points = Nsteps_left*dist/length_left;

        length_left = length_left - dist;
        Nsteps_left = Nsteps_left - time_points;

        for(int t = 0; t < time_points; t++) {
            if (time_points != 1) current_time = double(t)/double(time_points-1);
            else current_time = 1;

            newx = x1*pow(1-current_time,2) + 2*x2*(1-current_time)*current_time + x3*pow(current_time,2);
            newy = y1*pow(1-current_time,2) + 2*y2*(1-current_time)*current_time + y3*pow(current_time,2);
            newt = t1*(1-current_time) + t3*current_time;

            newiv.AddPoint(newx, newy, newt);
        }
    }

    // now, add the final line
    x1 = tempiv.X(temp_length-2);
    y1 = tempiv.Y(temp_length-2);
    t1 = tempiv.T(temp_length-2);

    x2 = tempiv.X(temp_length-1);
    y2 = tempiv.Y(temp_length-1);
    t2 = tempiv.T(temp_length-1);

    //time_points = Nsteps*sqrt(pow(x1-x2,2)+pow(y1-y2,2))/length;
    time_points = Nsteps-newiv.Length();

    for(unsigned int t = 0; t < time_points; t++) {
        if (time_points != 1) current_time = double(t)/double(time_points-1);
        else current_time = 1;

        newx = x1*(1-current_time) + x2*current_time;
        newy = y1*(1-current_time) + y2*current_time;
        newt = t1*(1-current_time) + t2*current_time;

        newiv.AddPoint(newx, newy, newt);
    }

    return newiv;
}


//New version of the quadratic bezier interpollation
InputVector BezierInterpolationV2(InputVector& iv, unsigned int Nsteps)
{
    const unsigned int nPoints = iv.Length();
    const unsigned int nBezPoints = 3*nPoints - 4;

    //no fancy interpolation necessary for one or two letter words
    if (nPoints <= 2)
        return SpatialInterpolation(iv,Nsteps);

    //create a new input vector with the bezier control points included
    InputVector bezIV;

    bezIV.AddPoint(iv.X(0),iv.Y(0),iv.T(0));
    for(unsigned int i = 1; i < nPoints-1; i++)
    {
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
    for(unsigned int i = 2; i < nBezPoints; i+=3)
    {
        //Create the next input vector segment to be bezier interpolated
        InputVector bezSeg;
        bezSeg.AddPoint(bezIV.X(i-1),bezIV.Y(i-1),bezIV.T(i-1));
        bezSeg.AddPoint(bezIV.X(i),bezIV.Y(i),bezIV.T(i));
        bezSeg.AddPoint(bezIV.X(i+1),bezIV.Y(i+1),bezIV.T(i+1));

        int nSegSteps = int(Nsteps*((1.5*dist2next(bezIV,i-1))/bezIV.SpatialLength()));
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
InputVector BezierSloppyInterpolationV2(InputVector& iv, unsigned int Nsteps)
{
    const unsigned int nPoints = iv.Length();
    const unsigned int nBezPoints = 2*nPoints - 1;

    //no fancy interpolation necessary for one or two letter words
    if (nPoints <= 2)
        return SpatialInterpolation(iv,Nsteps);

    //create a new input vector with the bezier control points included
    InputVector bezIV;

    bezIV.AddPoint(iv.X(0),iv.Y(0),iv.T(0));
    for(unsigned int i = 1; i < nPoints; i++)
    {
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
    for(unsigned int i = 2; i < nBezPoints-1; i+=2)
    {
        //Create the next input vector segment to be bezier interpolated
        InputVector bezSeg;
        bezSeg.AddPoint(bezIV.X(i-1),bezIV.Y(i-1),bezIV.T(i-1));
        bezSeg.AddPoint(bezIV.X(i),bezIV.Y(i),bezIV.T(i));
        bezSeg.AddPoint(bezIV.X(i+1),bezIV.Y(i+1),bezIV.T(i+1));

        int nSegSteps = int(Nsteps*((1.5*dist2next(bezIV,i-1))/bezIV.SpatialLength()));
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


//Helper function for the bezier interpolation.
//This function simply returns the list of points from a quadratic bezier interpolation
//of three control points.
InputVector QuadraticBezierInterpolation(InputVector& iv, unsigned int Nsteps)
{
    InputVector newIV;

    for(unsigned int i = 0; i <= Nsteps; i++)
    {
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
InputVector CombineInputVectors(std::vector<InputVector> IVvect)
{
    InputVector newIV;

    for(unsigned int i = 0; i < IVvect.size(); i++)
    {
        if(i==0)
        {
            for(unsigned int j=0; j < IVvect[i].Length(); j++)
            {
                newIV.AddPoint(IVvect[i].X(j),IVvect[i].Y(j),IVvect[i].T(j));
            }
        }

        else
        {
            for(unsigned int j=1; j < IVvect[i].Length(); j++)
            {
                newIV.AddPoint(IVvect[i].X(j),IVvect[i].Y(j),IVvect[i].T(j));
            }
        }
    }

    return newIV;
}


//Helper function that returns the distance between point i in input vector iv and the next point in iv
double dist2next(InputVector& iv, unsigned int i)
{
    return sqrt(pow(iv.X(i+1)-iv.X(i),2) + pow(iv.Y(i+1)-iv.Y(i),2));
}


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
		// begin by adding the first line
		time_points = round(Nsteps*sqrt(pow(a_x[i]-a_x[i+1],2)+pow(a_y[i]-a_y[i+1],2))/length);
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
	
	// we will draw line from 0->1, and then curve from 1->3, 3->5, 5->7, then (n-1)->n
	// we will thus be splitting up into 2 straight lines, and (n-3)/2.
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
			time_points = round(Nsteps*sqrt(pow(x1-x2,2)+pow(y1-y2,2))/length);
		
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

			time_points = round(Nsteps*(sqrt(pow(x1-x2,2)+pow(y1-y2,2))+sqrt(pow(x2-x3,2)+pow(y2-y3,2)))/length);

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

    return newiv;
}

// Basic Quadratic Interpolation
InputVector BezierSloppyInterpolation(InputVector& iv, unsigned int Nsteps) {
    const double length = iv.SpatialLength();
    const unsigned int points = iv.Length();

	if (points <= 2) return SpatialInterpolation(iv, Nsteps);

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
	
	time_points = round(Nsteps*sqrt(pow(x1-x2,2)+pow(y1-y2,2))/length);

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

		time_points = round(Nsteps*(sqrt(pow(x1-x2,2)+pow(y1-y2,2))+sqrt(pow(x2-x3,2)+pow(y2-y3,2)))/length);

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

	//time_points = round(Nsteps*sqrt(pow(x1-x2,2)+pow(y1-y2,2))/length);
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
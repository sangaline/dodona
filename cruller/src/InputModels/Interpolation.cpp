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
InputVector BezierInterpolation(InputVector& iv, unsigned int Nsteps) {
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

	// we will draw line from 0->1, and then curve from 1->3, 3->5, 5->7, then (n-1)->n
	// we will thus be splitting up into 2 straight lines, and (n-3)/2.
	// for X points -> Y curves; 5 -> 2, 7 -> 3, 9 -> 4. So, split 100 by (n-1)/2

	int temp_length = tempiv.Length();
	int pt_per = Nsteps/(points-1);

	double current_time = 0;
	double newx,newy,newt;

	// begin by adding the first line
	x1 = tempiv.X(0);
	y1 = tempiv.Y(0);
	t1 = tempiv.T(0);

	x2 = tempiv.X(1);
	y2 = tempiv.Y(1);
	t2 = tempiv.T(1);

    for(int t = 0; t < pt_per/2; t++) {
		current_time = double(t*2)/double(pt_per);
		
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

	    for(unsigned int t = 0; t < pt_per; t++) {
			current_time = double(t)/double(pt_per);
		
			newx = x1*pow(1-current_time,2) + 2*x2*(1-current_time)*current_time + x3*pow(current_time,2);
			newy = y1*pow(1-current_time,2) + 2*y2*(1-current_time)*current_time + y3*pow(current_time,2);
			newt = t1*pow(1-current_time,2) + 2*t2*(1-current_time)*current_time + t3*pow(current_time,2);

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

	int pt_left = Nsteps-newiv.Length();

    for(unsigned int t = 0; t < pt_left; t++) {
		current_time = double(t)/double(pt_left);
		
		newx = x1*(1-current_time) + x2*current_time;
		newy = y1*(1-current_time) + y2*current_time;
		newt = t1*(1-current_time) + t2*current_time;

		newiv.AddPoint(newx, newy, newt);
	}

    return newiv;
}
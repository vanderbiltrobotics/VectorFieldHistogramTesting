//
// Created by Phil on 2/3/2018.
//
#ifndef VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP
#define VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP

#include <math.h>
#include <iostream>
#include "VFHPather.h"
#include "Utils.h"
// #include "gnuplot-iostream.h"
#include "GNUPlot.h"



class RobotTest{
private:
    VFHPather pather; //Object used to store the grid/map of obstacles
    discretePoint currentPosition; //Stores the location of the robot
    double currentSpeed; // linear distance per timestep
    double maxTurnSpeed; // angle per timestep
    double currentAbsoluteAngle; //absolute
    // double[][] world

public:
    //RobotTest
    //Constructor for the RobotTest class
    // init_x: (int)
    // init_y: (int)
    RobotTest(int init_x, int init_y, double angle_init, double speed_init):
    // pather(20, 50, 100, 0.1) // iMax = 10, jMax = 10??
        pather() // iMax = 10, jMax = 10??
    {
        currentPosition.x = init_x;
        currentPosition.y = init_y;
        currentAbsoluteAngle = angle_init;
        currentSpeed = speed_init;
        pather.updateRobotPosition(currentPosition);
    }

    ~RobotTest()
    {
        // delete pather; // FIXME: no delete method in them
        // delete currentPosition; // FIXME: no delete method in them
    }

    //main function per timestep
    void move()
    {
        updateRobotPosition();
        updateRobotAngle();
        updateRobotSpeed();
    }

    void updateRobotPosition()
    {
        currentPosition.x += currentSpeed * cos(currentAbsoluteAngle);
        currentPosition.y += currentSpeed * sin(currentAbsoluteAngle);
        // std::cout<<"trying to update the robot position.\n";
        pather.updateRobotPosition(currentPosition);
        // pather.generateHistogram();
    }

    void updateRobotAngle()
    {
      // currentAbsoluteAngle = pather.getMinHistAngleBetweenAngles(currentPosition, currentAbsoluteAngle, maxTurnSpeed);
        // std::cout<<"trying to update the robot angle.\n";
        currentAbsoluteAngle = pather.computeTravelDirection();
    }

    // TODO: currently speed is always 1 width per timestep
    void updateRobotSpeed()
    {
        currentSpeed = 1;
    }

    void talk()
    {
        std::cout<<"currentPosition is "<<currentPosition.x<<", "<<currentPosition.y<<"\n";
    }

    void draw(GNUPlot plot)
    // void draw()
    {
        int iMax = pather.getIMax();
        int jMax = pather.getJMax();
        std::vector<int> x;
        std::vector<int> y;
        // std::iota(std::begin(x), std::end(x), 0); //0 is the starting number



        for(int i = 0; i < iMax; ++i)
        {
            for(int j = 0; j < jMax; ++j)
            {
                // std::cout << pather.getCellValue(i, j) << ' ';
                if(pather.getCellValue(i, j) == 1)
                {
                    std::cout << "pushing obstacle at (" << i << ", " << j << ")\n";
                    x.push_back(i);
                    y.push_back(j);
                }
            }
        }

        std::cout << "x.size() = " << x.size();
        std::cout << "y.size() = " << y.size();

        std::vector<std::vector<int>> v;
        v.push_back(x);
        v.push_back(y);



        // Gnuplot g2;

        std::cout << "window 2: user defined points" << std::endl;

        std::vector<std::string> script;
        script.push_back("set terminal qt");
        script.push_back("reset");
        script.push_back("plot v");

        GNUPlot plotter;
        plotter.open();
        plotter.execute(script);

        getchar(); // prevent graph to close

        plotter.write("exit");
        plotter.flush();
        plotter.close();
        // try {
        //
        //   // Don't forget to put "\n" at the end of each line!
        // 	// gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
        // 	// '-' means read from stdin.  The send1d() function sends data to gnuplot's stdin.
        // 	// gp << "plot '.' with vectors title 'x', '-' with vectors title 'y'\n";
        //
        //   // plot.set_style("points").set_xrange(0, iMax).set_yrange(0, jMax).plot_xy(x, y, "points");
        //
        //   // plot.reset_plot();
        // }
        //
        // catch (GnuplotException ge)
        // {
        //     std::cout << ge.what() << std::endl;
        // }


        // g2.plot_xy();
        // int** objectGrid = pather.getObjectGrid();
        // objectGrid[currentPosition.x][currentPosition.y] = 1;

        // int nrows = sizeof objectGrid / sizeof objectGrid[0];

        // int ncols = sizeof objectGrid[0] / sizeof(int);

        // std::cout<<"objectGrid: nrows = "<<nrows<<". ncols = "<<nrows;
        //
        // for (int i = 0; i < nrows; ++i)
        // {
        //     for (int j = 0; j < ncols; ++j)
        //     {
        //         std::cout << objectGrid[i][j] << ' ';
        //     }
        //     std::cout << std::endl;
        // }
    }
  };
#endif //VECTORFIELDHISTOGRAMTESTING_ROBOTTEST_CPP

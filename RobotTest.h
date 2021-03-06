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
#include "Plotter.h"



// TODO: I don't quite understand why you needed an entire new class for the 
//  execution of the program. You have something against the main method?  - Josh
class RobotTest {
private:
    HistogramGrid grid;
    PolarHistogram hist;
    VFHPather pather; // Object used to store the grid/map of obstacles
    discretePoint currentPosition; //Stores the location of the robot
    double currentSpeed; // linear distance per timestep
    double maxTurnSpeed; // angle per timestep
    double currentAbsoluteAngle; //absolute
    Plotter plotter;
    // double[][] world

public:
    //RobotTest
    //Constructor for the RobotTest class
    // init_x: (int)
    // init_y: (int)
    RobotTest(discretePoint initPos, double angle_init, double speed_init): grid("../map.txt", initPos), hist(32),
                                                                            pather(hist, &grid, 100, 1, 5, 15),
                                                                            currentPosition(initPos),
                                                                            currentSpeed(speed_init),
                                                                            currentAbsoluteAngle(angle_init)
    // pather(20, 50, 100, 0.1) // iMax = 10, jMax = 10??
         // iMax = 10, jMax = 10??
    {
        pather.updateRobotPosition(currentPosition);
        grid.setTargetLoc({50,50});
    }

//    ~RobotTest()
//    {
//        // delete pather; // FIXME: no delete method in them
//        // delete currentPosition; // FIXME: no delete method in them
//    }

    //main function per timestep
    void move()
    {
        updateRobotAngle();
        updateRobotSpeed();
        updateRobotPosition();
    }

    void updateRobotPosition()
    {
        currentPosition.x += currentSpeed * cos(currentAbsoluteAngle*M_PI/180);
        currentPosition.y += currentSpeed * sin(currentAbsoluteAngle*M_PI/180);
        // std::cout<<"trying to update the robot position.\n";
        pather.updateRobotPosition(currentPosition);
        // pather.generateHistogram();
    }

    void updateRobotAngle()
    {
      // currentAbsoluteAngle = pather.getMinHistAngleBetweenAngles(currentPosition, currentAbsoluteAngle, maxTurnSpeed);
        // std::cout<<"trying to update the robot angle.\n";
        currentAbsoluteAngle = pather.computeTravelDirection();
        std::cout << "Desired travel angle: " << currentAbsoluteAngle << "\n";
        //hist.printHistogram();
    }

    // TODO: currently speed is always 1 width per timestep
    void updateRobotSpeed()
    {
        currentSpeed = 2;
    }

    void talk()
    {
        std::cout<<"currentPosition is "<<currentPosition.x<<", "<<currentPosition.y<<""
                " Desired Position is " << grid.getTargetLoc().x << ", " << grid.getTargetLoc().y << "\n";
    }

    void draw()
    // void draw()
    {
        int iMax = pather.getIMax();
        int jMax = pather.getJMax();
        std::vector<discretePoint> positions;
        // std::iota(std::begin(x), std::end(x), 0); //0 is the starting number
        positions.push_back(grid.getRobotLoc());
        for(int i = 0; i < iMax; ++i)
        {
            for(int j = 0; j < jMax; ++j)
            {
                //std::cout << pather.getCellValue(i, j) << ' ';
                if(pather.getCellValue(i, j) == 1)
                {
                    //positions.push_back(grid.getRobotLoc());
                    positions.push_back({i, j});
                    //std::cout << "pushing obstacle at (" << i << ", " << j << ")\n";
                }
            }

        }
        plotter.plot(positions);
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

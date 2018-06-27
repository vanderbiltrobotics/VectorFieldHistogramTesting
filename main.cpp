#include <iostream>
#include "HistogramGrid.h"
#include "PolarHistogram.h"
#include "Utils.h"
#include "VFHPather.h"
#include "RobotTest.h"

#include <chrono>
#include <thread>
#include "Plotter.h"


int getIndex(int a, int n) {
    return ((a % n) + n) % n;
}

int main()
{
  // Start the robot at (30,30)
  discretePoint init;
  init.x = 30;
  init.y = 30;

  // TODO: Draw a thick line out of the center of the robot to indicate which 
  //  direction it's traveling.  - Josh
  // double angle_init = 0.78; // pi/4
  double angle_init = 90; // != pi/4
  double speed_init = 0.5;

  // TODO: I don't quite understand why you needed an entire new class for the 
  //  execution of the program. You have something against the main method?  - Josh
  RobotTest bot(init, angle_init, speed_init);

  int timestep = 100;


  for(int i = 0; i < timestep; ++i)
  {
    std::cout<<i<<"th timestep: ";
    bot.move();
    bot.talk();
    // bot.draw(gnuplot);
    bot.draw();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "\n\n\n\n\n\n";
  }
//    // VFHPather pather;
//    // discretePoint robotLoc;
//    // robotLoc.x = 25;
//    // robotLoc.y = 25;
//
//    // pather.updateRobotPosition(robotLoc);
//    //pather.generateHistogram();
//    // std:: cout << pather.computeTravelDirection();

}

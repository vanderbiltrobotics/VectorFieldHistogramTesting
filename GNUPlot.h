/**
 * GNUPlot.h
 */


#ifndef GNUPLOT_H
#define GNUPLOT_H

#include <cstdio>
#include <cstdlib>
#include <ostream>
#include <string>
#include <vector>


/**
 * \brief A class for sending plot commands to GNU-Plot.
 * Pulled from the following GitHub repo: 
 * https://github.com/ccerhan/gnuplot-cpp-interface
 * \author ccerhan on GitHub. 
 */
class GNUPlot
{
private:
  FILE* _pipe;

public:
  /// Initialize this class. 
  /// This does NOT begin gnuplot; open() must be called afterward to send 
  /// plot commands. All commands before then will be ignored.
  GNUPlot();
  ~GNUPlot();

  bool isOpened() const;

  /// Open the GNUPlot application.
  void open();
  /// Sends all data in the output file's buffer to the file. 
  void flush();
  /// Closes the file.
  void close();
  /// Write to the GNU-Plot file. See here for a quick command guide: http://www.gnuplot.info/docs_4.0/gpcard.pdf
  void write(const char *line);
  /// Write to the GNU-Plot file. See here for a quick command guide: http://www.gnuplot.info/docs_4.0/gpcard.pdf
  void write(const std::string &line);
  /// Write to the GNU-Plot file. See here for a quick command guide: http://www.gnuplot.info/docs_4.0/gpcard.pdf
  void execute(const std::vector<std::string> &script);
};

#endif //GNUPLOT_H

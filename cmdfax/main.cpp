#include <stdio.h>
#include "Options.h"
#include "CmdFax.h"

#include "FaxImage.h"


FaxImage faxImage;


int main(int argc, char * argv[]) {
Options options;
CmdFax  cmdfax;

  options.read(argc, argv);
  if (options.verbose > 0)
    printf("cmdfax v2.3\n");
  options.printOptions();

  if (cmdfax.init(&options)) {
    if (cmdfax.run() == 0) {
      if (faxImage.height() > 0) {
        if (options.turnLeft)
          faxImage.turnLeft();
        if (options.turnRight)
          faxImage.turnRight();
        return !faxImage.write(options.OutputFilename);
      }
    }
  }
  return 1;
}

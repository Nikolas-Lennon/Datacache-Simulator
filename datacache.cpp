/**********************************************************
 *       Name: Nikolas Lennon                              *
 *                                                         *
 *                                                         *
 *    Compile: "gcc -g -o datacache datacache.c"           *
 *                                                         *
 *                                                         *
 ***********************************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>


//-------------LET'S CREATE A TYPEDEF--------------//

struct Cache {

  unsigned int address;
  unsigned int vBit;
  unsigned int setLine;
  unsigned int leastRecent;
  unsigned int dBit;
  unsigned int lruTag;
  unsigned int lruIndex;
  unsigned int lruOffset;
  //unsigned int tagLength;

};

unsigned int numBlocks = 0;
unsigned int Sets = 0;
unsigned int numSets = 0; //amount of entries (blocks)
unsigned int setSize = 0;//associativity
unsigned int lineSize = 0; //block_size in bytes
unsigned int blockSize = 0;

const int MAXNUMSETS = 8192;
const int MAXSETSIZE = 8;

float tAccesses = 0.00;

float hCount = 0.00;
float mCount = 0.00;

//----------------END OF TYPEDEF------------------//


//--------------OLD UNION TESTING---------------//
//union {
//unsigned int x;
//
//struct {
//unsigned int offset : 3;
//unsigned int index : 1;
//unsigned int tag : 4;
//}tio;
//
//}hex;

int main(int argc, const char** argv) {
  
  const unsigned int MAXSIZE = 8;
  const unsigned int MAXLINE = 8;
  
  std::string ADDRESS;
  unsigned int lineCount = 1;
  char access;
  char line[MAXSIZE];
  unsigned int datasize;
  unsigned int hexaddress;
  std::string garbage;
  char delimeter(':');
  
  if (argc != 1) {
    std::cerr << argv[1] << ": No such file or directory.";
    exit(EXIT_FAILURE);
  }
  
  //-----------------OPEN FIRST FILE---------------//
  std::ifstream fin("trace.config", std::ios::in);
  fin.seekg(std::ios::beg);
  
  if (!fin) {
    std::cerr << "Unable to open file." << argv[1] << std::endl;
    exit(EXIT_FAILURE);
  }
  
  while (std::getline(fin, garbage, delimeter))
  {
    fin >> numSets;
    std::getline(fin, garbage, delimeter);
    fin >> setSize;
    std::getline(fin, garbage, delimeter);
    fin >> lineSize;
  }
  
  if (numSets % 2 == 0) {
    
    
    if ((setSize == 1) || (setSize == 2) || (setSize == 4) || (setSize == 8)) {
      fin.close();
      
      //-------------OPEN SECOND FILE---------------//
      std::ifstream fin2("trace.dat", std::ios::in);
      fin2.seekg(std::ios::beg);
      
      if (!fin2) {
        std::cerr << "Unable to open file!";
        exit(EXIT_FAILURE);
      }
      
      std::cout.seekp(std::ios::beg);
      
      std::cout << "Cache Configuration\n" << std::endl;
      std::cout << "   " << numSets << " " << setSize << "-way set associative entries\n" << "   of line size " << lineSize << " bytes\n\n" << std::endl;
      std::cout << "Results for Each Reference\n" << std::endl;
      std::cout << "Ref  "
                << "Access "
                << "Address    "
                << "Tag   "
                << "Index "
                << "Offset "
                << "Result "
                << "Memrefs"
                << std::endl;
      
      std::cout << "---- "
                << "------ "
                << "-------- "
                << "------- "
                << "----- "
                << "------ "
                << "------ "
                << "-------"
                << std::endl;
      
      
      
      while (fgets(line, MAXLINE, stdin)) {
        
        sscanf(line, "%c:%d:%x", &access, &datasize, &hexaddress);

        fin2.close();
        //hex.x = hexaddress;
        unsigned int tag = 0;
        unsigned int index = 0;
        unsigned int offset = 0;
        unsigned int references = 0;
        
        if (hexaddress % datasize == 0) {
          
          std::cout << std::setw(4) << lineCount << " ";
          
          if (access == 'R') {
            std::cout << std::setw(6) << "read";
            std::cout << " ";
            std::cout << std::setw(8) << std::hex << hexaddress;
          }
          else if (access == 'W') {
            std::cout << std::setw(6) << "write";
            std::cout << " ";
            std::cout << std::setw(8) << std::hex << hexaddress;
          }
          
          unsigned int hexCopy = hexaddress;
          unsigned int mask = lineSize - 1;
          unsigned int mask2 = numSets - 1;
          
          offset = hexCopy & mask;
          
          hexCopy = hexCopy >> static_cast<unsigned int>(ceil(log2(lineSize)));
          
          index = hexCopy & mask2;
          
          hexCopy = hexCopy >> static_cast<unsigned int>(ceil(log2(numSets)));
          
          tag = hexCopy;
          
          //tag = hexaddress >> 4;
          //index = hexaddress >> 6;
          //offset = hexaddress & 4; 
          
          std::cout << std::setw(8) << std::dec << tag;
          std::cout << std::setw(6) << std::dec << index;
          std::cout << std::setw(7) << std::dec << offset;
          
          unsigned int least = 0;
          
          
          std::cout << "\n";
          tAccesses++;
        }
        else {
          std::cerr << "line " << lineCount
                    << " has misaligned reference at address " << std::hex << hexaddress
                    << " for size " << datasize;
          std::cerr << "\n";
          continue;
        }
        lineCount++;
        
      }
      
      
      //------------PRINT OUT FILE-----------------//
      
      std::cout << "\n" << std::endl;
      
      std::cout << "Simulation Summary Statistics\n"
                << "-----------------------------\n"
                << "Total hits       : " << hCount << '\n'
                << "Total misses     : " << mCount << '\n'
                << "Total accesses   : " << tAccesses << '\n'
                << "Hit ratio        : " << std::fixed << std::setprecision(4) << hCount / tAccesses << '\n'
                << "Miss ratio       : " << std::fixed << std::setprecision(4) << mCount / tAccesses << '\n';
    }
    else {
      std::cerr << "reference 1 is illegal: Number of sets: " << numSets
                << "\n\nreference 2 is illegal: Set size: " << setSize
                << "\n\nreference 3 is illegal: Line size: " << lineSize;
    }
  }
  else {
    std::cerr << "numsets not a power of two";
  }
}

#include <iostream>
#include <unistd.h> // usleep(t) where t is time in microseconds
#include <vector>
#include <cmath>
#include <string.h>
#include <random>

/*
Take user input and generate a firework based on that input, could ask for how many fireworks to create or size of firework

Methods: 
  create_matrix: 
    resizes matrix for a 2D vector based on the user input 
    and fills the matrix with periods in a circular shape
  
  explode_firework_asterisk:
    takes a pointer to the matrix created in create_matrix 
    method and replaces the periods with asterisks in a 
    circular pattern with a delay so it seems like there is a
    gradual change occuring
    
  explode_firework_degree_symbol: 
    same idea as asterisk method, except now replacing 
    the asterisks with degree symbols

  dissipate_firework:
    again, same idea. Goes through the matrix in a circular
    pattern and replaces degree symbols with white spaces
    to make it seem like the firework is gradually dissipating

  print_matrix:
    method to print the firework as changes are made to it

  Global Variables: 
    degree_symbol:
      the degree symbol (°) is kind of a tricky character to 
      print. I found I could get around this by using the 
      unicode character 

    clear_output:
      Uses ANSI escape codes to clear the console. 

    colors (array):
      global array that stores different ANSI codes (I think)
      to change the color of the output text
*/

/*
##############################################################################

Some prototypes of what the "firework" in the console could look like:
            
   \       /                    °                                .  
    \     /                  °*°o.*°                       . . . . . . .
     \   /                °*.o*.*°.*.°o*°                . . . . . . . . .
      \ /              °.*o°°*..*o.°.*o*°.o°           . . . . . . . . . . .
------ ° -------      °.*.o*-.°*.o°.*.°°*.°o°          . . . . . . . . . . . 
      / \             °.*.o*°.o.°**°.o°**°*°o        . . . . . . . . . . . . .  
     /   \             °°.o*.°*°.o°*.°.o*°.°           . . . . . . . . . . .
    /     \              *o°.o°**°o..*°o.°             . . . . . . . . . . . 
   /       \               °o*.*o°.*..*°                 . . . . . . . . .
                             °.o*°o.°                      . . . . . . .
                                °                                .


###############################################################################
*/

//UNICODE
const std::string degree_symbol = "\u00B0"; // unicode character for degree symbol
const std::string clear_output = "\033[2J\033[H"; //use this to clear the output and display updated matrix while iterating through

//unicode for colored text
const std::string red = "\033[31m"; //this can be used in print statements to change color of output text
const std::string blue = "\033[94m";
const std::string white = "\033[97m";
const std::string cyan = "\033[36m";

std::string colors[4] = {red, blue, white, cyan};


class Fireworks{
  public: 
  void create_matrix(std::vector<std::vector<std::string>>&firework_matrix, int size) {
    
    firework_matrix.resize(size, std::vector<std::string>(size));

    int radius = size/2;
    int hypotenuse;
    
    for (int i = 0; i < size; ++i) {
      /*i represents row number, j represents column number
        if the current index of the array falls within the radius of the circle then set it to a period
        otherwise set it to a whitespace
        think of distance from center of a specific point on a circle as a triangle, if x^2 + x^2 <= radius^2
        then the point is within the radius of the circle. Just using the pythagorean theorem
      */
        for (int j = 0; j < size; ++j) {
          hypotenuse = std::pow(i-radius, 2) + std::pow(j-radius, 2);
          if(hypotenuse <= std::pow(radius, 2)) {
            firework_matrix[i][j] = '.';
          }
          else {
            firework_matrix[i][j] = ' ';
          } 
        }
      }
  }

  void explode_firework_asterisk(std::vector<std::vector<std::string>>&firework_matrix, int curr_radius, int radius) { 
    //need to start off at circle of radius 0 and move outward gradually exploding each circle

    int hypotenuse=0;
    
    //we need to start from the center of the matrix, curr radius = 0 corresponds to the center, not row 0
    //firework_matrix[radius][radius] represents the middle of the matrix
    for (int j = 0; j <= curr_radius; ++j) {
        for (int i = 0; i < curr_radius; ++i) {
          hypotenuse = std::pow(i, 2) + std::pow(j, 2);
          if(hypotenuse <= std::pow(curr_radius, 2)){
            firework_matrix[radius-j][radius-i] = '*';
            firework_matrix[radius+j][radius+i] = '*';
            firework_matrix[radius-j][radius+i] = '*';
            firework_matrix[radius+j][radius-i] = '*';
          }
        } //end of inner for loop
    }
  }

  void explode_firework_degree_symbol(std::vector<std::vector<std::string>>&firework_matrix, int curr_radius, int radius) { 
    //need to start off at circle of radius 0 and move outward gradually exploding each circle
    
    int hypotenuse=0;
    
    //we need to start from the center of the matrix, so start at matrix[radius][radius] and move outward

    for (int j = 0; j <= curr_radius; ++j) {
        for (int i = 0; i < curr_radius; ++i) {
          hypotenuse = std::pow(i, 2) + std::pow(j, 2);
          if(hypotenuse <= std::pow(curr_radius, 2)){
            firework_matrix[radius-j][radius-i] = degree_symbol;
            firework_matrix[radius+j][radius+i] = degree_symbol;
            firework_matrix[radius-j][radius+i] = degree_symbol;
            firework_matrix[radius+j][radius-i] = degree_symbol;
          }
        } //end of inner for loop
    }
  }

  void dissipate_firework(std::vector<std::vector<std::string>>&firework_matrix, int curr_radius, int radius) { 

    int hypotenuse;

    //firework_matrix[radius][radius] represents the center of the matrix
    
    for (int j = 0; j <= curr_radius; ++j) {
        for (int i = 0; i < curr_radius; ++i) {
          hypotenuse = std::pow(i, 2) + std::pow(j, 2);
          if(hypotenuse <= std::pow(curr_radius, 2)){
            firework_matrix[radius-j][radius-i] = " ";
            firework_matrix[radius+j][radius+i] = " ";
            firework_matrix[radius-j][radius+i] = " ";
            firework_matrix[radius+j][radius-i] = " ";
          }
        } //end of inner for loop
    }
  }

  int gen_random_num(int min, int max) {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(min, max);

    int random_integer = distribution(gen);
    return random_integer;
  }

  void print_matrix(std::vector<std::vector<std::string>>& matrix) {

  for (const auto& row : matrix) {
    int color = gen_random_num(0, 3); 
    /*
    generate a random number between 0 and 3, then using the global
    array "colors" generate a random color for the text while it's 
    printing by doing colors[random_num] to randomize the index
    */
     for (std::string cell : row) {
          std::cout << colors[color] <<cell << " ";
      }
      std::cout << std::endl;
    }
  }
};

  void simulate_firework(std::vector<std::vector<std::string>>&fw_matrix, int firework_size, int radius, Fireworks fw1) { 
    
   std::cout << clear_output;//clears the output to make way for updated firework
   fw1.print_matrix(fw_matrix);
   int curr_radius=0;
    
    while(curr_radius < (radius+1)) { // replace "." with "*" 
      fw1.explode_firework_asterisk(fw_matrix, curr_radius, radius);
      usleep(450000);
      curr_radius++;
      std::cout << clear_output;
      fw1.print_matrix(fw_matrix);
    }
    
    curr_radius=0; //reset to 0 for next while loop of asterisk->degree_symbol

    while(curr_radius < (radius+1)) {
      fw1.explode_firework_degree_symbol(fw_matrix, curr_radius, radius);
      usleep(450000);
      curr_radius++;
      std::cout << clear_output;
      fw1.print_matrix(fw_matrix);
    }

    curr_radius=0;

    while(curr_radius < (radius+1)) {
      fw1.dissipate_firework(fw_matrix, curr_radius, radius);
      usleep(450000);
      curr_radius++;
      std::cout << clear_output;
      fw1.print_matrix(fw_matrix);
    }
    
  usleep(550000);
  std::cout << clear_output;
    
  }

int main() {

  int firework_size;

  std::cout<<"Enter the size of the firework you want to see (MUST BE AN ODD number between 1-15):";
  std::cin>>firework_size;
  int radius = (firework_size/2);
  
  if(firework_size < 1 || firework_size > 15 || (firework_size % 2 == 0)){
    std::cout<<"Invalid. Please run the program again and enter an odd number 1-15\nProgram Ended";
  }
    
  else {
    
    std::vector<std::vector<std::string>> fw_matrix;
    Fireworks fw1;
    fw1.create_matrix(fw_matrix, firework_size);
    simulate_firework(fw_matrix, firework_size, radius, fw1);
  }  
}
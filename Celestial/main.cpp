#include <string.h>
#include "Functions/Basic/plus.h"
#include "Functions/Basic/minus.h"
#include "Functions/Basic/times.h"
#include "Functions/Basic/divide.h"

int main() {

	int input;
	int current = 0;
	int sum;

	for (int i = 0; i < 255; i++) {
		std::cout << "(add) Fill in a number: \n";
		std::cin >> input;
		sum = input + current;
		current = sum;
		std::cout << "Your answer is now: " << current << std::endl;
	}
}
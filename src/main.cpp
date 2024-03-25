#include <MainClass.hpp>

using namespace std;

int	main(int ac, char **av){
	if (ac != 2){
		cout << "Usage: ./scop <scene>\n";
		cout << "1: 42\n";
		cout << "2: Cube\n";
		cout << "3: Teapot\n";
		cout << "4: Teapot2\n";
		cout << "5: All objects" << endl;
		return (0);
	}
	if (strlen(av[1]) != 1 || av[1][0] < '1' || av[1][0] > '5'){
		cout << '\"' << av[1] << "\" is not a valid scene" << endl;
		return (0);
	}

	MainClass	mainClass(av[1][0] - '1');

	try {
		mainClass.run();
	} catch(const exception& e){
		cerr << e.what() << '\n';
		return (1);
	}
	return (0);
}

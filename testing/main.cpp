using namespace std;

#include <bits/stdc++.h>

int main(){

	ofstream file;

	file.open("saidas/grafico_convergencia");

	file << "OI" << endl;

	file.close();
	file.open("saidas/grafico_convergencia", ios::trunc);

	file << "Tchau" << endl;

	file.close();


}

//   double sum =  sum( sin(xx) * ( sin(ii*xx^2/pi) )^(2*m) )
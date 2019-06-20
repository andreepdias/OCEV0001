using namespace std;

#include <bits/stdc++.h>

int main(){


	for(int i = 1; i <= 5000; i++){
		printf("%d. %.5lf\n", i, pow(i, 1.0 / 3) / pow(5000, 1.0 / 3));
	}

}

//   double sum =  sum( sin(xx) * ( sin(ii*xx^2/pi) )^(2*m) )
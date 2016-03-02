#pragma once
#include <Windows.h>
#include <map>
#include <vector>

// PIN IDs are used as 
const char NOPIN = -1;
const char A_PIN = 'A';
const char B_PIN = A_PIN + 1;
const char C_PIN = B_PIN + 1;
const char D_PIN = C_PIN + 1;
const char E_PIN = D_PIN + 1;
const char F_PIN = E_PIN + 1;
const char G_PIN = F_PIN + 1;
const char H_PIN = G_PIN + 1;
const char I_PIN = H_PIN + 1;
const char J_PIN = I_PIN + 1;
const char K_PIN = J_PIN + 1;
const char L_PIN = K_PIN + 1;
const char M_PIN = L_PIN + 1;
const char N_PIN = M_PIN + 1;
const char O_PIN = N_PIN + 1;
const char P_PIN = O_PIN + 1;
const char Q_PIN = P_PIN + 1;
const char R_PIN = Q_PIN + 1;
const char S_PIN = R_PIN + 1;
const char T_PIN = S_PIN + 1;
const char U_PIN = T_PIN + 1;
const char V_PIN = U_PIN + 1;
const char W_PIN = V_PIN + 1;
const char X_PIN = W_PIN + 1;

// Direction definition
//       2       1
//        \     /
//         \   /
//   3 ---   6  --- 0
//         /   \ 
//        /	    \
//       4       5

// Neighbourhood map
//    A    B    C    D    E    F    
// G    H    I    J    K    L
//    M    N    O    P    Q    R
// S    T    U    V    W    X

const std::map< char, std::vector<char> > s_neighbourhood =
{
	{ A_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ B_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ C_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ D_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ E_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ F_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ G_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ H_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ I_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ J_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ K_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ L_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ M_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ N_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ O_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ P_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ Q_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ R_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ S_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ T_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ U_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ V_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ W_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ X_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
};


class CPin
{
private:
	unsigned char _name;
	std::vector<
};
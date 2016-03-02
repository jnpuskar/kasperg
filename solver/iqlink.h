#pragma once
#include <vector>
#include <map>

// IQ LINK game representation - place 12 pieces with 36 PIN openings to a board with only 24 PINs

// PIN IDs are used as 
const char NOPIN = -1;
const char A_PIN = 0;
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

// Direction & position definition
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
	//PIN  ,    ----- NEIGHBOUR IN EACH DIRECTION ----
	// ID        0      1      2      3      4      5      
	{ A_PIN, { B_PIN, NOPIN, NOPIN, NOPIN, G_PIN, H_PIN } },
	{ B_PIN, { C_PIN, NOPIN, NOPIN, A_PIN, H_PIN, I_PIN } },
	{ C_PIN, { D_PIN, NOPIN, NOPIN, B_PIN, I_PIN, J_PIN } },
	{ D_PIN, { E_PIN, NOPIN, NOPIN, C_PIN, J_PIN, K_PIN } },
	{ E_PIN, { F_PIN, NOPIN, NOPIN, D_PIN, K_PIN, L_PIN } },
	{ F_PIN, { NOPIN, NOPIN, NOPIN, E_PIN, L_PIN, NOPIN } },
	// ID        0      1      2      3      4      5      
	{ G_PIN, { H_PIN, A_PIN, NOPIN, NOPIN, NOPIN, M_PIN } },
	{ H_PIN, { I_PIN, B_PIN, A_PIN, G_PIN, M_PIN, N_PIN } },
	{ I_PIN, { J_PIN, C_PIN, B_PIN, H_PIN, N_PIN, O_PIN } },
	{ J_PIN, { K_PIN, D_PIN, C_PIN, I_PIN, O_PIN, P_PIN } },
	{ K_PIN, { L_PIN, E_PIN, D_PIN, J_PIN, P_PIN, Q_PIN } },
	{ L_PIN, { NOPIN, F_PIN, E_PIN, K_PIN, Q_PIN, R_PIN } },
	// ID        0      1      2      3      4      5      
	{ M_PIN, { N_PIN, H_PIN, G_PIN, NOPIN, S_PIN, T_PIN } },
	{ N_PIN, { O_PIN, I_PIN, H_PIN, M_PIN, T_PIN, U_PIN } },
	{ O_PIN, { P_PIN, J_PIN, I_PIN, N_PIN, U_PIN, V_PIN } },
	{ P_PIN, { Q_PIN, K_PIN, J_PIN, O_PIN, V_PIN, W_PIN } },
	{ Q_PIN, { R_PIN, L_PIN, K_PIN, P_PIN, W_PIN, X_PIN } },
	{ R_PIN, { NOPIN, NOPIN, L_PIN, Q_PIN, X_PIN, NOPIN } },
	// ID        0      1      2      3      4      5      
	{ S_PIN, { T_PIN, M_PIN, NOPIN, NOPIN, NOPIN, NOPIN } },
	{ T_PIN, { U_PIN, N_PIN, M_PIN, S_PIN, NOPIN, NOPIN } },
	{ U_PIN, { V_PIN, O_PIN, N_PIN, T_PIN, NOPIN, NOPIN } },
	{ V_PIN, { W_PIN, P_PIN, O_PIN, U_PIN, NOPIN, NOPIN } },
	{ W_PIN, { X_PIN, Q_PIN, P_PIN, V_PIN, NOPIN, NOPIN } },
	{ X_PIN, { NOPIN, R_PIN, Q_PIN, W_PIN, NOPIN, NOPIN } },
};

// 12 Piece definitions - each spans 3 PINS
// Take 1st PIN and place the piece in such a way that the 2nd PIN is in direction 0. 
// Mark direction of the piece between 1st and 2nd(3 bits) and 2nd and 3rd PIN (3bits) 
// and also mark all occupated positions at each PIN (3 x 3 bits) => 16 bits is fine 
// to hold it
//  -Pin1-positions-  -Pin2-positions-  -Pin3-positions- -Pin1-Pin2-Direction-  -Pin2-Pin3-Direction- 
//    b14 ... b12       b11 ...  b9        b8 ...  b6        b5 ... b3             b2 ... b0
const unsigned short BLUE = 0x0000;
const unsigned short RED = 0x0000;
const unsigned short GREEN = 0x0000;
const unsigned short YELLOW = 0x0000;
const unsigned short PURPLE = 0x0000;
const unsigned short ORANGE = 0x0000;
const unsigned short BROWN = 0x0000;
const unsigned short PINK = 0x0000;
const unsigned short KHAKI = 0x0000;
const unsigned short WINE = 0x0000;
const unsigned short LGREEN = 0x0000;
const unsigned short LBLUE = 0x0000;

// Piece rotation is done via modular arithmetics mod 6. EVery piece at given PIN can be 
// placed at most in 6 different direction for every PIN and since it is 2D object it can be flipped.
// Totalling to 3 * 6 * 2 = 36 different positions at given PIN

// This function performs rotation and possible flip with rotation based on parameter 0 .. 35 
unsigned short Rotate(unsigned short piece, char position);

// Occupancy map - keeps state of empty/full positions for all PINs
// For every PIN we need to store info about each direction (0 .. 5) and also about the center of the PIN (6).
// These 7 values can be represented by 3 bits. We have 24 PINS => 72 bits. We will keep the state in 2 8-byte values.
// Each PIN has 3 bits according to its order A_PIN (b0 - b2), ..., X_PIN(b70 - b72)
//Example:
//unsigned long long ulOCMapHigh = 0x0000000000000000;
//unsigned long long ulOCMapLow = 0x0000000000000000;

bool IsPinAvailable(unsigned long long ulMapHigh, unsigned long long ulMapLow, char pin);

// Tests if the piece can be placed in given position
// To Do to be decided at next step...
bool IsPlaceable(unsigned long long ulMapHigh, unsigned long long ulMapLow, char pin, unsigned short piece, char rotation);

// Modifies 
void PlacePiece(unsigned long long& ulMapHigh, unsigned long long& ulMapLow, char pin, unsigned short piece, char rotation);

// Consumption map - keeps state of which pieces are available for next move
// We have 12 pieces that can be used. Each piece has its own index of bit.

char FindFreePiece(unsigned short consumption_map);
void ConsumePiece(unsigned short consumption_map, char piece);
void FreePiece(unsigned short consumption_map, char piece);
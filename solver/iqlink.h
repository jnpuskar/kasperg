#pragma once
#include <vector>
#include <map>

// IQ LINK game representation - place 12 pieces with 36 PIN openings to a board with only 24 PINs

// PIN IDs are used as 
const unsigned char NOPIN = 0xFF;
const unsigned char A_PIN = 0;
const unsigned char B_PIN = A_PIN + 1;
const unsigned char C_PIN = B_PIN + 1;
const unsigned char D_PIN = C_PIN + 1;
const unsigned char E_PIN = D_PIN + 1;
const unsigned char F_PIN = E_PIN + 1;
const unsigned char G_PIN = F_PIN + 1;
const unsigned char H_PIN = G_PIN + 1;
const unsigned char I_PIN = H_PIN + 1;
const unsigned char J_PIN = I_PIN + 1;
const unsigned char K_PIN = J_PIN + 1;
const unsigned char L_PIN = K_PIN + 1;
const unsigned char M_PIN = L_PIN + 1;
const unsigned char N_PIN = M_PIN + 1;
const unsigned char O_PIN = N_PIN + 1;
const unsigned char P_PIN = O_PIN + 1;
const unsigned char Q_PIN = P_PIN + 1;
const unsigned char R_PIN = Q_PIN + 1;
const unsigned char S_PIN = R_PIN + 1;
const unsigned char T_PIN = S_PIN + 1;
const unsigned char U_PIN = T_PIN + 1;
const unsigned char V_PIN = U_PIN + 1;
const unsigned char W_PIN = V_PIN + 1;
const unsigned char X_PIN = W_PIN + 1;

const unsigned char PIN_CNT = X_PIN + 1;
// Direction & position definition
//       2       1
//        \     /
//         \   /
//   3 ---   6  --- 0
//         /   \ 
//        /	    \
//       4       5

// PIN Neighbourhood map
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

const unsigned char PIECES_CNT = 12;

// Piece rotation is done via modular arithmetics mod 6. EVery piece at given PIN can be 
// placed at most in 6 different direction for every PIN and since it is 2D object it can be flipped.
// Totalling to 3 * 6 * 2 = 36 different positions at given PIN

const unsigned char POSITIONS_CNT = 36;

// This function performs rotation and possible flip with rotation based on parameter 0 .. 35 
unsigned short Rotate(unsigned short piece, char position);

// Occupancy vector - keeps state of empty/full positions for all PINs + piece id that occupies the PIN
// For every PIN we need to store info about each direction (0 .. 5) and also about the center of the PIN (6).
// These 7 values must keep id of the piece(12 = 4 bits) occupying it. We have 24 PINS => 24 * 7 * 4 bits.
// After all a vector of 24 unsigned longs(32 bits) will be fine.

bool IsPinAvailable(std::vector<unsigned long> occupance, char pin);
// Tests if the piece can be placed in given position and outputs new occupance if so
bool IsPlaceable(std::vector<unsigned long> occupance, std::vector<unsigned long>& new_occupance, unsigned char pin, unsigned short piece, unsigned char rotation);

// Availability flag - keeps state of which pieces are available for next move
// We have 12 pieces that can be used. Each piece has its own index of bit.
inline bool IsPieceAvailable(unsigned short availability, unsigned char piece) { return 0 != (availability & (1 << piece)); }
unsigned short ConsumePiece(unsigned short availability, unsigned char piece) { return availability & (~(1 << piece)); }
unsigned short FreePiece(unsigned short availability, unsigned char piece) { return availability | (1 << piece); }
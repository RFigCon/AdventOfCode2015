#include "../global_in.hpp"

#if defined (_WIN64) || defined (_WIN32)
    #include <winsock2.h>   //to use htohl() and ntohl()
#else
    #include <arpa/inet.h>  //to use htohl() and ntohl()
#endif

using namespace std;

static const string RES_DAY4 = "resources/day4/";


//****************FOR DEBBUGING*********************
static char get_hex_code(unsigned char val){
    val = val & 0xF;
    switch(val){
        case 0xA:
            return 'A';
        case 0xB:
            return 'B';
        case 0xC:
            return 'C';
        case 0xD:
            return 'D';
        case 0xE:
            return 'E';
        case 0xF:
            return 'F';
        default:
            return '0' + val;
    }
}

static void printhex(basic_string<unsigned char> hex){

    for(int i = 0; i<hex.size(); i++){
        cout << get_hex_code( hex[i]>>4 );
        cout << get_hex_code( hex[i]  ) << " ";
    }
    cout << endl;
}

static void printhex(unsigned int hex){

    unsigned char mem[4];
    *(unsigned int*)&mem[0] = hex;
    for(int i = 0; i<4; i++){
        cout << get_hex_code( mem[i]>>4 );
        cout << get_hex_code( mem[i]  ) << " ";
    }
    cout << endl;
}
//**************************************************


static string get_key_start(string file_name){

    ifstream file(RES_DAY4 + file_name);

    stringstream buff;
    buff << file.rdbuf();

    file.close();
    return buff.str();
}

static bool done(basic_string<unsigned char> hash, int zeroes){
    int half = zeroes/2;

    for(int i = 0; i<half; i++){
        
        if(hash[i] != 0x00){
            return false;
        }
    }

    if(zeroes%2 != 0){
        if((unsigned char)hash[half] > 0x0F) return false;
    }
    
    
    return true;
}

static basic_string<unsigned char> make_key(string start, int numb){
    
    if(numb > 0){
        start.append( to_string(numb) );
    }
    
    const int BLOCK_SIZE = 64;   //512/8
    const int LEN_INFO_SIZE = 8;      //64/8

    //Pad key:
    long msg_size = start.size();

    int remainder = msg_size % BLOCK_SIZE;
    int new_size = msg_size;

    if( (BLOCK_SIZE - remainder) <= LEN_INFO_SIZE){
        new_size += ( (BLOCK_SIZE - remainder) + BLOCK_SIZE);
    }else{
        new_size += (BLOCK_SIZE - remainder);
    }

    basic_string<unsigned char> padded_key(new_size, 0);
    
    for(int i = 0; i<msg_size; i++){
        padded_key[i] = (unsigned char)start[i];
        //cout << (unsigned char)start[i];
    }
    // cout << endl;
    //Add one bit to end (or byte 0x80), then pad with zeroes until the last 8 bytes which hold the size of the original message (key) modulo 2^64

    int idx = msg_size;
    padded_key[idx] = 0x80;

    msg_size *= 8;

    idx = new_size - LEN_INFO_SIZE; //In this scenario the length of the key will probably always fit into just 4 bytes,
    *(int *)&padded_key[idx] = msg_size; //so we can just use this cheat instead of setting every byte one by one in a loop!

    return padded_key;
}


// s specifies the per-round shift amounts
static const unsigned int s[64] = {  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,      //0..15
                            5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,      //16..31
                            4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,      //32..47
                            6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21       //48..63
                        };

// Constants:
// static const unsigned int K[64] = {  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
//                             0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
//                             0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
//                             0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
//                             0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
//                             0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
//                             0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
//                             0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
//                             0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
//                             0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
//                             0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
//                             0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
//                             0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
//                             0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
//                             0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
//                             0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
//                         };

static unsigned int K[64];
static void calcK(){
    double s, pwr;
	int i;

	pwr = pow(2.0, 32);
	for (i = 0; i<64; i++) {
		s = fabs(sin(1.0 + i));
		K[i] = (unsigned)(s * pwr);
	}
}

static unsigned int leftrotate(unsigned int bits, unsigned int n){
    
    unsigned int mask = ~0;         //e.g. = 1111 1111 (8 bits instead of 32)
    mask = mask << (n);          //e.g. = 1111 1100
    mask = ~mask;                   //e.g. = 0000 0011

    unsigned int tmp = mask & ( bits >> (32-n));
    return (bits<<n)+tmp; 
}

static basic_string<unsigned char> md5_hash(basic_string<unsigned char> key){
    unsigned int a = ntohl(0x01234567);
    unsigned int b = ntohl(0x89abcdef);
    unsigned int c = ntohl(0xfedcba98);
    unsigned int d = ntohl(0x76543210);

    //In this case the key will never be longer than 512 bits / 64 bytes
    //so we can just proccess the first block

    //break chunk/block into sixteen 32-bit (4 bytes) words M[i], 0 ≤ i ≤ 15

    unsigned int M[16];
    for(int i = 0; i<16; i++){
        int j = i*4;

        M[i] = *(unsigned int *)&key[j];

    }

    // Initialize hash value for this chunk:
    unsigned int A = a;
    unsigned int B = b;
    unsigned int C = c;
    unsigned int D = d;

    // Main loop:
    for (unsigned int i = 0;  i < 64; i++){
        unsigned int F, g;
        if (i < 16){
            F = (B & C) | ((~B) & D);
            g = i;
        }else if (i < 32){
            F = (B & D) | (C & (~D));
            g = (5*i + 1)%16;
        }else if (i < 48){
            F = (B ^ C) ^ D;
            g = (3*i + 5)%16;
        }else /*if (i < 64)*/{
            F = C ^ (B | (~D));
            g = (7*i)%16;
        }
        // Be wary of the below definitions of a,b,c,d
        F = F + A + K[i] + M[g];  // M[g] must be a 32-bit block
        A = D;
        D = C;
        C = B;
        B = B + leftrotate(F, s[i]);
    }
    // Add this chunk's hash to result so far:
    a = a + A;
    b = b + B;
    c = c + C;
    d = d + D;

    //Build Hash

    basic_string<unsigned char> hash(16, 0);

    *(unsigned int *)&hash[0] = (a);
    *(unsigned int *)&hash[4] = (b);
    *(unsigned int *)&hash[8] = (c);
    *(unsigned int *)&hash[12] = (d);

    // for(int i = 0; i<16; i++){
    //     cout << (unsigned int)hash[i] << " "; 
    // }
    // cout << endl;

    return hash;
}

static unsigned int get_lowest_number(int zeroes){
    calcK();
    string key_start = get_key_start("input.txt");
    key_start.resize( key_start.size()-1 ); //remove new line character

    unsigned int result = 1;

    basic_string<unsigned char> hash;

    do{
        basic_string<unsigned char> real_key = make_key(key_start, result);

        hash = md5_hash( real_key );

    }while( !done(hash, zeroes) && result++ < 10000000 );

    return result;
}

namespace MD5{
    void write_answers(){
        unsigned int result = get_lowest_number(5);
        cout << "\t-Lowest number that results in five zeroes: " << result << endl;

        result = get_lowest_number(6);
        cout << "\t-Lowest number that results in six zeroes: " << result << endl;
    }
}

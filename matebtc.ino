// serial comm
#define READ 0x80

// LED display
#define NDIGITS 4
#define NMAP 25
unsigned long lastep = micros(); // last digit change
char digits[NDIGITS]; // currently displayed content
char digit = 0; // currently lit digit
char digitmap[NMAP][9] = { // digits
	"01111110",
	"10110000",
	"21101101",
	"31111001",
	"40110011",
	"51011011",
	"61011111",
	"71110000",
	"81111111",
	"91111011",
	";1000010", // upper-left corner (⸢)
	"?0011000", // lower-right corner (⸥)
	"H0110111",
	".0000000", // empty digit
	"q1000000", // clock states
	"w1100000",
	"e1110000",
	"r1111000",
	"t1111100",
	"z1111110",
	"a0111110",
	"s0011110",
	"d0001110",
	"f0000110",
	"g0000010"
};

void setup() {
	for (int i = 2; i <= 12; i++) {
		pinMode(i, OUTPUT);
	}
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	pinMode(A2, INPUT);
	digits[0] = ';'; // startup logo (⸢HS⸥)
	digits[1] = 'H';
	digits[2] = '5';
	digits[3] = '?';
	Serial.begin(9600);
}

byte last_a1 = LOW; // CLK
byte last_a2 = LOW; // RST

byte recv_digit_pos = 0;
byte recv_buf = 0;
byte recv_bit_pos = 0;

void loop() {
	stepleds();
	int a1 = digitalRead(A1);
	int a2 = digitalRead(A2);
	if (a1 != last_a1) {
		Serial.print("A1 changed: ");
		Serial.println(a1, DEC);
		if (a1 == HIGH) {
			recv_buf <<= 1;
			if (digitalRead(A0) == HIGH) recv_buf |= 1;
			Serial.print("Next recv_buf: 0x");
			Serial.println(recv_buf, HEX);
			if (++recv_bit_pos == 8) {
				recv_bit_pos = 0;
				digits[recv_digit_pos++] = recv_buf;
				if (recv_digit_pos == NDIGITS) recv_digit_pos = 0;
				Serial.print("recv_digit_pos: ");
				Serial.println(recv_digit_pos, DEC);
			}
			Serial.print("recv_bit_pos: ");
			Serial.println(recv_bit_pos, DEC);
		}
		last_a1 = a1;
	}
	if (a2 != last_a2) {
		Serial.print("A2 changed: ");
		Serial.println(a2, DEC);
		if (a2 == HIGH) {
			recv_digit_pos = 0;
			recv_buf = 0;
			recv_bit_pos = 0;
			Serial.println("Reset done");
		}
		last_a2 = a2;
	}
}

void stepleds() {
	unsigned long now = micros();
	if (now - lastep > 4000) {
		lastep = now;
	} else {
		return; // every digit is lit for 4 ms
	}
	for (int i = 0; i < NDIGITS; i++) { // select digit
		digitalWrite(i + 2, digit == i ? HIGH : LOW);
	}
	for (int i = 0; i < NMAP; i++) { // look up the character
		if (digitmap[i][0] == digits[digit]) { // if found...
			for (int j = 0; j < 7; j++) { // ...set it segment by segment
				digitalWrite(j + 6, digitmap[i][j + 1] == '1' ? LOW : HIGH);
			}
		}
	}
	if (++digit == NDIGITS) digit = 0; // next digit
}

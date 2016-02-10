
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: put your names here
// Date Created: 1/24/2015 
// Last Modified: 3/6/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "../../tm4c123gh6pm.h"
#include "../TExaS.h"
#include "../../drivers/dac.h"
#include "../../brandonware/BrandonMML.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void wait(uint32_t time);



//
#define R	0
#define	C4 261
#define	D4 293
#define	E4 329
#define	F4 349
#define	G4 392
#define	A4 440
#define	B4 493
#define C5 523
//

const uint16_t pianoNotes[8] = {C4,D4,E4,F4,G4,A4,B4,C5};	//C5 should never be reached
const uint8_t nullsong[] = "";
const uint8_t ievan1[] = "k\x01""t\x6c""v\x0f""l\x08""o\x04""aagf\x10""f\x10""ec\x10""c\x10""cegg\x10""g\x10""fefddfl\x10""aaaal\x08""gfel\x10""cccce\x08""ggg\x08""f\x08""eeffd\x08""d\x04""l\x08""<a>dd.l\x10""effddd\x08""ffl\x08""eccefddd\x10""d\x10""<a>dd.l\x10""efrdd\x08""ddfaaagfre\x08""f\x08""drd\x08""ddl\x08""aagfel\x10""cc\x08""cceggggffeef\x08""dd&d\x08"".dl\x08""aagfecl\x10""ccceggggf\x08""e\x08""f\x08""ddd\x04""l\x08""<a>ddd\x10""e\x10""fdd\x10""d\x10""fec\x10""c\x10""cefdd\x04""<al\x10"">ddd\x08""def\x08""ddd\x08""ffaaggffe\x08""e\x08""d\x08""d\x08""r\x08""aaaag\x08""ffecccc\x08""e\x08""g\x08""ggf\x08""eefdddd\x08""f\x08""aaaal\x08""gfecce\x10""g\x10""rgfef\x10""d\x10""dd\x04""<al\x10"">dddddef\x08""drd\x08""f\x08""e\x08""ccc\x08""eef\x08""drd\x04""<a\x08"">dddddef\x08""drd\x08""ffa\x08""ggf\x08""e\x08""f\x08""drd\x04""aaaal\x08""gfecce\x10""g\x10""rl\x10""ggl\x08""feee\x10""e\x10""egl\x10""aaaag\x08""f\x08""e\x08""c\x08""c\x08""egr\x08""g\x08""f\x08""e\x08""eed\x08""d\x08""r\x08""<a\x08"">dddddef\x08""d\x08""d\x08""f\x08""e\x08""ccc\x08""e\x08""f\x08""d\x08""d\x04""<a\x08"">dddddef\x08""ddd\x08""ffaag\x08""f\x08""e\x08""e\x08""d\x08""d\x08""ffa\x08""aag\x08""f\x08""e\x08""crc\x08""e\x08""g\x08""ggf\x08""eredddd\x08""f\x08""aaaal\x08""gfeccl\x10""egl\x08""rgfel\x10""fdd\x08""d\x04""j\x00""";
const uint8_t ievan2[] = "k\x01""t\x6c""v\x0d""l\x04""o\x04""d<<aa>c<a>cd<a>d<aa>c<a>cddd<a>d<aa>cd<a>d<a>d<aa>cddd<aa>c<a>cd<a>d<aa>c<a>cddd<a>d<aa>cd<a>d<a>d<aa>cL\x08""cddrl\x01""rrrrl\x10""r\x08""ddd\x02"".d\x08""ccc\x04"".drd\x04""r\x08""ddd\x04"".ddd\x04""r\x08""ccc\x04"".ddd\x08""d\x08""l\x04""r<aa>c<a>cd<a>d<aa>c<a>cl\x10""ddd\x08""d\x04""l\x01""d&dd&d\x02"".l\x10""r\x08""ffj\x00""";
const uint8_t servant[] = "k\x01""t\x90""v\x0f""l\x08""o\x04""fedfe\x04"".edc<b>dc\x02""<b\x04"".bb>dc<ba\x04"">c+\x04""d\x04""e\x04""ffffr\x02""<<<a\x01""&a\x01""a\x01""&a\x01""t\x78""r\x02""r\x04""r>a+\x10"">c\x10""c+.c+.g+g+.f+.fl\x10""d+\x08""d+d+&d+d+f\x08""f\x04""f\x08""d+\x08""c+\x08"".c+\x08"".f\x08""d+\x04""<a+\x08"">c\x08""c+\x08""c+c+&&c+c+d+\x08""c\x04""r\x08""<a+>cc+\x08"".c+\x08"".g+\x08""g+\x08"".f+\x08"".f\x08""d+\x08""d+d+d+d+f\x08""f\x04""f\x08""d+\x08""c+\x08"".c+\x08"".d+\x08""c\x04""c+\x08""c\x08""<a+\x08""a+a+a+a+g+\x08""a+\x04""r\x08"">a+>cc+\x08"".c+\x08"".g+\x08""g+\x08"".f+\x08"".f\x08""d+\x08""d+d+&d+d+f\x08""f\x04""f\x08""d+\x08""c+\x08"".c+\x08"".f\x08""d+\x04""<a+\x08"">c\x08""c+\x08""c+c+&c+c+d+\x08""c\x04""r\x08""<a+>cc+\x08"".c+\x08"".g+\x08""g+\x08"".f+\x08"".f\x08""d+\x08""d+d+&d+d+f\x08""f\x04""f\x08""d+\x08""c+\x08"".c+\x08"".d+\x08""c\x04""c+\x08""c\x08""<a+\x08""a+a+&a+a+g+\x08""a+\x02""f\x08""ff&f\x08""f\x08""d+\x08""dc&cdd+\x08""<a+\x08""a+a+&a+a+>c\x08""d\x02""d+\x08""d+d+&d+\x08""d+\x08""d\x08""c<a+&a+\x08"">d\x08""c\x08""cc&c<a+>d\x08""c\x02""f\x08""ff&f\x08""f\x08""d+\x08""dc&ccd\x08""<a+\x08"".a+&a+a+>c\x08""d\x04""d\x08""c\x08""<a+\x08"".a+&a+\x08"">d\x08""c\x04""<g\x08""a\x08""a+\x08"".a+&a+a+>c\x08""<a\x04""r\x08""a+>cd\x04""d\x04""c\x08"".c\x08"".d\x08""d+\x08""d+d&d\x08""c\x08""d\x04""r\x08""<a+>cd\x04""d\x04""c\x08"".c\x08"".d\x08""d+\x08""d+d&d\x08""c\x08""<a+\x04""r\x08""a+>cd\x04""d\x04""c\x08"".c\x08"".d\x08""d+\x08""d+d&ddg\x08""f\x04""c\x08""d\x08""d+\x04""d+\x04""f\x08"".g\x08"".a\x08""a+\x04"".a\x08""a\x02""l\x08""c+.c+.cc\x04""rl\x10""<a+>cl\x08""c+.c+.g+g+.f+.fl\x10""d+\x08""d+d+d+d+f\x08""f\x04""l\x08""fd+c+.c+.fd+\x04""<a+>cl\x10""c+\x08""c+c+&c+c+d+\x08""f\x04""r\x08""<a+>cj\x00""";
const uint8_t flea[] = "k\x01""t\x98""v\x0f""l\x08""o\x04""r\x02""rdff+gr>ered\x04""c+c\x04""r\x04""r<ag+gf+r>d<c>dc\x04""<b&b\x04""r\x04""rgf+feg>c<a&a>cdr<ga+>d+c&cef\x04""g\x01""r\x04""ggagd+\x10""d.cre<e>ed\x04""c+c\x04""r\x04""r<ag+gf+r>d<c>dc\x04""<b&b\x04""r\x04""rgf+feg>c<a&a>cdr<ga+>d+c&cd+f\x04""g\x01""r\x04""ggagd+\x10""d.crr\x04""r\x02""j\x00""";
const uint8_t woods[] = "k\x01""t\x96""V\x0f""l\x08""o\x04""r\x01""fab\x04""fab\x04""fab>ed\x04""<b>c<bge\x02""rdege\x02""r\x04""fab\x04""fab\x04""fab>ed\x04""<b>cge<b\x02""r>e<bge\x02""r\x04""fga\x04""b>cd\x04""<bge\x02""r\x04""def\x04""gab\x04"">cde\x02""r\x04""<fga\x04""b>cd\x04""<bge\x02""r\x04""gfagba>c<b>dcedfeg\x10""fd\x10""e\x04"".l\x10""eee\x04""eee\x04""eee\x08""e\x08""f\x02""j\x00""";
const uint8_t mario[] = "k\x01""t\x69""v\x0f""o\x04"">e\x20""r\x20""e\x20""r\x10"".e\x20""r\x10"".c\x20""r\x20""e\x20""r\x10"".g\x20""r\x04"".r\x10"".c\x20""r\x08""r\x20""<g\x20""r\x08""r\x20""e\x20""r\x08""r\x20""a\x20""r\x10"".b\x20""r\x10"".a+\x20""r\x20""a\x20""r\x10"".g\x20""r\x10"">e\x20""r\x20""g\x10""r\x20""a\x20""r\x10"".f\x20""r\x20""g\x20""r\x10"".e\x20""r\x10"".c\x20""r\x20""d\x20""r\x20""<b\x20""r\x08""r\x20"">c\x20""r\x08""r\x20""<g\x20""r\x08""r\x20""e\x20""r\x08""r\x20""a\x20""r\x10"".b\x20""r\x10"".a+\x20""r\x20""a\x20""r\x10"".g\x20""r\x10"">e\x20""r\x20""g\x10""r\x20""a\x20""r\x10"".f\x20""r\x20""g\x20""r\x10"".e\x20""r\x10"".c\x20""r\x20""d\x20""r\x20""<b\x20""r\x04""r\x20"">g\x20""r\x20""f+\x20""r\x20""f\x20""r\x20""d+\x20""r\x10"".e\x20""r\x10"".<g+\x20""r\x20""a\x20""r\x20"">c\x20""r\x10"".<a\x20""r\x20"">c\x20""r\x20""d\x20""r\x08""r\x20""g\x20""r\x20""f+\x20""r\x20""f\x20""r\x20""d+\x20""r\x10"".e\x20""r\x10"".>c\x20""r\x10"".c\x20""r\x20""c\x20""r\x04""r\x10"".<g\x20""r\x20""f+\x20""r\x20""f\x20""r\x20""d+\x20""r\x10"".e\x20""r\x10"".<g+\x20""r\x20""a\x20""r\x20"">c\x20""r\x10"".<a\x20""r\x20"">c\x20""r\x20""d\x20""r\x08""r\x20""d+\x20""r\x08""r\x20""d\x20""r\x08""r\x20""c\x20""r\x02""r\x10"".g\x20""r\x20""f+\x20""r\x20""f\x20""r\x20""d+\x20""r\x10"".e\x20""r\x10"".<g+\x20""r\x20""a\x20""r\x20"">c\x20""r\x10"".<a\x20""r\x20"">c\x20""r\x20""d\x20""r\x08""r\x20""g\x20""r\x20""f+\x20""r\x20""f\x20""r\x20""d+\x20""r\x10"".e\x20""r\x10"".>c\x20""r\x10"".c\x20""r\x20""c\x20""r\xFF"".<g\x20""r\x20""f+\x20""r\x20""f\x20""r\x20""d+\x20""r\x10"".e\x20""r\x10"".<g+\x20""r\x20""a\x20""r\x20"">c\x20""r\x10"".<a\x20""r\x20"">c\x20""r\x20""d\x20""r\x08""r\x20""d+\x20""r\x08""r\x20""d\x20""r\x08""r\x20""c\x20""r\x04"".r\x10"".c\x20""r\x20""c\x20""r\x10"".c\x20""r\x10"".c\x20""r\x20""d\x20""r\x10"".e\x20""r\x20""c\x20""r\x10"".<a\x20""r\x20""g\x20""r\x08"".r\x20"">c\x20""r\x20""c\x20""r\x10"".c\x20""r\x10"".c\x20""r\x20""d\x20""r\x20""e\x20""r\x02""r\x20""c\x20""r\x20""c\x20""r\x10"".c\x20""r\x10"".c\x20""r\x20""d\x20""r\x10"".e\x20""r\x20""c\x20""r\x10"".<a\x20""r\x20""g\x20""r\x08"".r\x20"">e\x20""r\x20""e\x20""r\x10"".e\x20""r\x10"".c\x20""r\x20""e\x20""r\x10"".g\x20""r\x04"".r\x10"".c\x20""r\x08""r\x20""<g\x20""r\x08""r\x20""e\x20""r\x08""r\x20""a\x20""r\x10"".b\x20""r\x10"".a+\x20""r\x20""a\x20""r\x10"".g\x20""r\x10"">e\x20""r\x20""g\x10""r\x20""a\x20""r\x10"".f\x20""r\x20""g\x20""r\x10"".e\x20""r\x10"".c\x20""r\x20""d\x20""r\x20""<b\x20""r\x08""r\x20"">c\x20""r\x08""r\x20""<g\x20""r\x08""r\x20""e\x20""r\x08""r\x20""a\x20""r\x10"".b\x20""r\x10"".a+\x20""r\x20""a\x20""r\x10"".g\x20""r\x10"">e\x20""r\x20""g\x10""r\x20""a\x20""r\x10"".f\x20""r\x20""g\x20""r\x10"".e\x20""r\x10"".c\x20""r\x20""d\x20""r\x20""<b\x20""r\x08""r\x20"">e\x20""r\x20""c\x20""r\x10"".<g\x20""r\x08""r\x20""g+\x20""r\x10"".a\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""<a\x20""r\x08"".r\x20""b\x20""r\x10"">a\x20""r\x20""a\x10""r\x20""a\x20""r\x10""g\x20""r\x20""f\x10""r\x20""e\x20""r\x20""c\x20""r\x10"".<a\x20""r\x20""g\x20""r\x08"".r\x20"">e\x20""r\x20""c\x20""r\x10"".<g\x20""r\x08""r\x20""g+\x20""r\x10"".a\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""<a\x20""r\x08"".r\x20""b\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""f\x20""r\x10""e\x20""r\x20""d\x10""r\x20""c\x20""r\x04"".r\x10"".e\x20""r\x20""c\x20""r\x10"".<g\x20""r\x08""r\x20""g+\x20""r\x10"".a\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""<a\x20""r\x08"".r\x20""b\x20""r\x10"">a\x20""r\x20""a\x10""r\x20""a\x20""r\x10""g\x20""r\x20""f\x10""r\x20""e\x20""r\x20""c\x20""r\x10"".<a\x20""r\x20""g\x20""r\x08"".r\x20"">e\x20""r\x20""c\x20""r\x10"".<g\x20""r\x08""r\x20""g+\x20""r\x10"".a\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""<a\x20""r\x08"".r\x20""b\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""f\x20""r\x10""e\x20""r\x20""d\x10""r\x20""c\x20""r\x04"".r\x10"".c\x20""r\x20""c\x20""r\x10"".c\x20""r\x10"".c\x20""r\x20""d\x20""r\x10"".e\x20""r\x20""c\x20""r\x10"".<a\x20""r\x20""g\x20""r\x08"".r\x20"">c\x20""r\x20""c\x20""r\x10"".c\x20""r\x10"".c\x20""r\x20""d\x20""r\x20""e\x20""r\x02""r\x20""c\x20""r\x20""c\x20""r\x10"".c\x20""r\x10"".c\x20""r\x20""d\x20""r\x10"".e\x20""r\x20""c\x20""r\x10"".<a\x20""r\x20""g\x20""r\x08"".r\x20"">e\x20""r\x20""e\x20""r\x10"".e\x20""r\x10"".c\x20""r\x20""e\x20""r\x10"".g\x20""r\x04"".r\x10"".e\x20""r\x20""c\x20""r\x10"".<g\x20""r\x08""r\x20""g+\x20""r\x10"".a\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""<a\x20""r\x08"".r\x20""b\x20""r\x10"">a\x20""r\x20""a\x10""r\x20""a\x20""r\x10""g\x20""r\x20""f\x10""r\x20""e\x20""r\x20""c\x20""r\x10"".<a\x20""r\x20""g\x20""r\x08"".r\x20"">e\x20""r\x20""c\x20""r\x10"".<g\x20""r\x08""r\x20""g+\x20""r\x10"".a\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""<a\x20""r\x08"".r\x20""b\x20""r\x20"">f\x20""r\x10"".f\x20""r\x20""f\x20""r\x10""e\x20""r\x20""d\x10""r\x20""c\x20""j\x00""";
const uint8_t stillalive[] = "k\x01""t\x78""v\x0f""l\x08""o\x04""r\x02""gf+eef+rr\x04""r\x02""r\x04""r<a>gf+ee\x04""f+d\x04""e<a\x04""rr\x04""r\x04""ra>ef+g\x04""ec+\x04""d\x04"".e\x04""<aa\x04"">f+rrr\x02""gf+eef+rr\x04""r\x02""r\x04""r<a>gf+eer\x04""f+dr\x04""e<a\x04""rr\x04""r\x02"">e\x04""f+g&g\x04""ec+&c+\x04""der<a>defedcr\x04""<aa+>c\x04""f\x04""eddcdcc\x04""c\x04""<aa+>c\x04""f\x04""gfeddef\x04""f\x04""gaa+a+a\x04""g\x04""fgaag\x04""f\x04""dcdffe\x04""ef+f+&f+\x04""r\x04""r\x02""j\x00""";
const uint8_t cmajor[] = "k\x01""t\x78""o\x01""cdefgab>cdefgab>cdefgab>cdefgab>cdefgab>cdefgab>cdefgabj\x00""";
const uint8_t runninghell1[] = "k\x01""t\x9a""v\x0f""l\x08""o\x04""<<a\x01""a\x01""a\x02""aaa\x10""a\x10""a\x10""a\x10""a\x01""a\x01""a\x01""a\x01""a\x01""a>fd<a>fd<a>g+afdafd>c<gac<a+>c<a+>cc+dc>c<ba+ag+gf+o\x03""a>fd<a>fd<a>g+afdafd>c<gac<a+>c<a+>cc+dcf\x04""e\x04""d+\x04"">d+\x10""e\x10""fdfa+afd<a>gecgcegeg\x04"".f\x04"".e\x04""fdfdfdfdj\x00""";
const uint8_t runninghell2[] = "k\x01""t\x9a""v\x0f""l\x08""o\x04""<<drdadadadrdadadadrdadadadrdadadadrdadadadrdadadadrdadadaar\x10""ar\x10""ar\x10""ar\x10""ard\x04""dadadad\x04""dadadac\x04""cgcgcggg\x04""g\x04""e\x04""g>d\x04"".d\x04""d\x04""df\x04"".f\x04""f\x04""fe\x04"".e\x04""e\x04""eg\x04"".g\x04""g\x04""gf<a>f<a>f<a>f<a>e<g>e<g>e<g>e<g>c<g>c<g>c<g>c<g>f<a>f<a>f<a>f<aj\x00""";
const uint8_t clannad1[266] = "k\x01""t\x91""v\x0f""l\x08""o\x04""d\x04""c+d&dac+c+&c+\x04""r\x02""rc+d\x04""c+<b&babab\x04""r\x04""r\x02"">d\x04""c+d\x04""ac+c+&c+\x04""r\x04""<a\x04"".a&ag+b\x02"".&b\x02"".r\x04"">d\x04""c+d&dac+c+&c+\x04""r\x02""rc+d\x04""c+<b&babab\x04""r\x04""r\x02"">d\x04""c+d\x04""ac+c+c+\x04""de&e\x04""ef&f\x04""ga&aa+aa&af&f\x02""r\x04""a\x02""&ag\x04"">c&c\x04""<ff&f\x04"".ra\x02""&ag\x04"">d&d\x04""c<f&f\x04""f\x04""f\x02""&fgg+g&g\x04""d+c&cd+\x04""f&f\x01""&f\x02"".r\x04""f+\x02""&f+gae&e\x02"".>d<g&g\x02""rgae&e\x04"".dd\x02""j\x00""";
const uint8_t clannad2[317] = "k\x01""t\x91""v\x0f""l\x08""o\x04""<<<b>f+b>d\x04""<bf+<ba>ef+>c+\x02""<a<g>dgb\x04"">d<gd<gb>g\x02"">d<d<b>f+b>d\x04""<bf+<ba>ef+>c+\x02""<aeabe\x04""ereeabe\x04"".&e\x04""<b>f+b>d\x04""<bf+<ba>ef+>c+\x02""<a<g>dgb\x04"">d<gd<gb>g\x02"">d<d<b>f+b>d\x04""<bf+<ba+>faa+&a+faa+<a+>faa+&a+>a+f<a+>><a\x02"".ra\x10""a\x10""a\x04""arra\x04""rg\x04""grgr\x04""ga\x04""arra\x04""rg\x04""grgr\x04""gc+\x04""f\x04""a+\x04""rc&cd+ga+g>c<a+gf\x02"".fa+f\x01""da>d<a>e<a>f+<e&eg>c<b>ec<gcd\x02""<b>dgac\x04"".cd\x02""j\x00""";
const uint8_t ava1[] = "k\x01""v\x0f""t\x9a""o\x04""g+.r\x20""d+\x10"".g+\x08""&g+\x20""a+b\x10"".&b.&b\x20""e\x10"".b\x08""&b\x20"">c+d+\x10"".&d+\x08""&d+\x20""&d+\x10"".ed+f+c+d+\x08""&d+\x20""c+d+\x10"".c+\x08""&c+\x20""<b\x10"".g+&g+\x10""r\x10"".g+\x10"".g+\x08""&g+\x20""d+e\x10"".&e.&e\x20""e\x10"".f+\x08""&f+\x20""ed+\x10"".&d+\x08""&d+\x20""&d+\x10"".d+ef+a+f+\x08""&f+\x20""a+b\x10"".a+\x08""&a+\x20""f+\x10"".g+.r\x20""d+\x10"".g+\x08""&g+\x20""a+b\x10"".&b.&b\x20""e\x10"".b\x08""&b\x20"">c+d+\x10"".&d+\x08""&d+\x20""&d+\x10"".ed+f+c+d+\x08""&d+\x20""c+d+\x10"".c+\x08""&c+\x20""<b\x10"".g+&g+\x10""r\x10"".g+\x10"".g+\x08""&g+\x20""d+e\x10"".&e.&e\x20""e\x10"".f+\x08""&f+\x20""ed+\x10"".&d+\x08""&d+\x20""&d+\x10"".d+ef+a+f+\x08""&f+\x20""a+b\x10"".a+\x08""&a+\x20""f+\x10"".j\x00""";
const uint8_t cs1[] = "k\x01""t\x6c""v\x0f""l\x08""o\x04"">d+g+d+g+dg+dg+c+g+c+g+cg+cc+\x10"".d\x20""d+g+d+g+dg+dg+c+g+c+g+ba+g+rl\x10""d+\x08""fr\x20""f\x20""f+.g+\x20""f+.f\x20""d+\x08""r\x08""f+.g+\x20""f+.f\x20""d+\x08""<b\x08"">f+.f+\x20""f.d+\x20""f\x08""c+\x08""g+.g+\x20""f\x08""d+\x08""fr\x20""f\x20""f+.g+\x20""f+.f\x20""d+\x08""r\x08""f+.g+\x20""f+.f\x20""d+\x08""<b\x08"">f+.f+\x20""f.d+\x20""f\x08""c+\x08""g+.c+\x20""d\x08""a+\x08""r\x08""g+.a\x20""a+.b\x20"">c+\x08""r\x08""d+\x08""c+\x08""c+\x08""r\x08""d+\x08""c+\x08""<b\x08""r\x08""f+\x08""g+\x08""a\x08""a.g+\x20""a.g+\x20""a.g+\x20""a\x08""r\x08""r\x08""a\x08""g+\x08""g+.f+\x20""g+.f+\x20""g+.f+\x20""g+\x02""a+\x08""r\x08""g+.a\x20""a+.b\x20"">c+\x08""r\x08""d+\x08""c+\x08""c+\x08""r\x08""d+\x08""c+\x08""f+\x08""f+\x08""r\x04""l\x08""r<ef+a>c+rc+<b>c+\x02""r\x02""j\x00""";
const uint8_t cs2[] = "k\x01""t\x6c""v\x0d""l\x08""o\x04""<g+>d+<g+>d+<g+>d<g+>d<g+>c+<g+>c+<g+>c<g+a+\x10"".g+\x20""g+>d+<g+>d+<g+>d<g+>d<g+>c+<g+>c+f+fd+r<d+a+>d+<a+d+a+>d+<a+<b>f+<b>f+c+g+c+g+d+a+>d+<a+d+a+>d+<a+<b>f+<b>f+c+g+c+df+>c+<f+>c+<f>c+<f>c+<e>c+<e>c+<d+bd+bdadadadac+g+c+g+c+g+c+g+f+>c+<f+>c+<f>c+<f>c+<e>c+<e>c+<d+bd+bda>d<ada>d<ac+g+c+g+c+g+c+g+j\x00""";

const uint8_t axel1[475] = "v\x0f""t\x78""l\x08""o\x04""f\x04""g+.ff\x10""a+fd+f\x04"">c.<ff\x10"">c+c<g+f>cf<f\x10""d+d+\x10""cgf&f\x02""r\x02""f\x04""g+.ff\x10""a+fd+f\x04"">c.<ff\x10"">c+c<g+f>cf<f\x10""d+d+\x10""cgf&f\x02""r\x02""<<f\x04"">f.<d+>d+\x10""<c>c<d+f\x04"">f.r<c\x10"">cd+f<c+\x04"">c+.<d+>d+\x10""<c>c<d+f\x04""r\x04""r\x10"">f\x10""c<a+g+f\x04"">f.<d+>d+\x10""<c>c<d+f\x04"">f.r<d+\x10"">cd+f<c+\x04"">c+.<d+>d+\x10""<c>c<d+f\x04""r\x04""r\x10"">f\x10""c<a+g+>>f\x04""g+.ff\x10""a+fd+f\x04"">c.<ff\x10"">c+c<g+f>cf<f\x10""d+d+\x10""cgf&f\x02""r\x02""f\x04""g+.ff\x10""a+fd+f\x04"">c.<ff\x10"">c+c<g+f>cf<f\x10""d+d+\x10""cgf&f\x02""r\x02""r>ccc\x10""d+d+d+\x10""ddrccc\x10""d+d+\x10""dc\x04""r<g+g+g+g+\x10""a+a+a+a+\x10""a+>ccc<a+\x10"">cc.rrccc\x10""d+d+d+\x10""ddrccc\x10""d+d+\x10""dc\x04""r<g+g+g+g+\x10""a+a+a+a+\x10""a+>ccc<a+\x10"">cc.r";
const uint8_t axel2[300] = "v\x0f""t\x78""o\x04""l\x01""rrrrrrrrrrrrrrrrl\x08""<<f\x04"">f.<d+>d+\x10""<c>c<d+f\x04"">f.r<c\x10"">cd+f<c+\x04"">c+.<d+>d+\x10""<c>c<d+f\x04""r\x04""r\x10"">f\x10""c<a+g+f\x04"">f.<d+>d+\x10""<c>c<d+f\x04"">f.r<d+\x10"">cd+f<c+\x04"">c+.<d+>d+\x10""<c>c<d+f\x04""r\x04""r\x10"">f\x10""c<a+g+f\x04"">f.<d+>d+\x10""<c>c<d+f\x04"">f.r<c\x10"">cd+f<c+\x04"">c+\x04""<d+\x04"">d+\x04""<f\x04"">f\x04""r\x10""f\x10""c<a+g+f\x04"">f.<d+>d+\x10""<c>c<d+f\x04"">f.r<c\x10"">cd+f<c+\x04"">c+\x04""<d+\x04"">d+\x04""<f\x04"">f\x04""r\x10""f\x10""c<a+g+";
//const uint8_t* axel = {axel1,axel2);

const uint8_t* iv[] = {ievan1,ievan2};
const uint8_t* cs[] = {cs1,cs2};
const uint8_t* rh[] = {runninghell1,runninghell2};
const struct BMML_Song song_ievan = {2,(uint8_t**)iv};
const struct BMML_Song song_cavestory = {2,(uint8_t**)cs};
const struct BMML_Song song_runninghell = {2,(uint8_t**)rh};

uint8_t isSong = 0;			//0 for standard piano operation, 1 for song
uint8_t songIndex = 0;	//Index for song array

struct BMML_Track tracks[3];
struct BMML_TrackHolder trackHolder = {2,tracks,0};
#define SONG_NUM 6

const uint8_t* songs[] = {cmajor,ievan1,flea,woods,mario,stillalive,servant};

void SysTick_Handler()
{
  DisableInterrupts();
  BMML_HolderUpdate(&trackHolder,NVIC_ST_RELOAD_R);
  DAC_Out(trackHolder.output);
	GPIO_PORTF_DATA_R ^= 0x02;	//Heartbeat flip bit
	GPIO_PORTB_DATA_R ^= 0x08;	//Heartbeat flip bit
  EnableInterrupts();
}

void Heartbeat_Init()
{
	uint8_t var;
	
	//LED Heartbeat is PF1
	SYSCTL_RCGC2_R |= 0x20;			//Activate PortF
	var++;											//Clock set delay
	var--;
	GPIO_PORTF_AMSEL_R &= ~0x02;		//Clear xxxx xx0x for no analog
	GPIO_PORTF_PCTL_R &= ~0x000000F0;	//0 for PF1 for GPIO function
	GPIO_PORTF_DIR_R |= 0x02;			//Set xxxx xx1x for output
	GPIO_PORTF_AFSEL_R &= ~0x02;		//Clear to disable PortF alt functions
	GPIO_PORTF_DEN_R |= 0x02;			//Enable digital on PF1
	
	//Oscilloscope Heartbeat is PB3
	SYSCTL_RCGC2_R |= 0x02;			//Activate PortB
	var++;											//Clock set delay
	var--;
	GPIO_PORTB_AMSEL_R &= ~0x08;		//Clear xxxx 0xxx for no analog
	GPIO_PORTB_PCTL_R &= ~0x0000F000;	//0 for PB3 for GPIO function
	GPIO_PORTB_DIR_R |= 0x08;			//Set xxxx 1xxx for output
	GPIO_PORTB_AFSEL_R &= ~0x08;		//Clear to disable PortB alt functions
	GPIO_PORTB_DEN_R |= 0x08;			//Enable digital on PB3
}

int main(void){      
  //TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  PLL_Init();
  DAC_Init();
	Heartbeat_Init();

  // other initialization
	DisableInterrupts();
	NVIC_ST_CTRL_R = 0;	
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_RELOAD_R = (80000000)/(2000 * 108); //pitch 2000, 128 element -> reduced for better quality. I will improve the synth's ability
  NVIC_ST_CTRL_R = 0x0007;
  BMML_HolderInit(&trackHolder,NVIC_ST_RELOAD_R);
  BMML_HolderLoadSong(&trackHolder,(struct BMML_Song*)&song_runninghell,NVIC_ST_RELOAD_R);
  DAC_SetEffect(0xFFFFFFFF);
  //BMML_TrackLoadSong(&(trackHolder.tracks[0]),NVIC_ST_RELOAD_R,(uint8_t*)axel1);
  //BMML_TrackLoadSong(&(trackHolder.tracks[1]),NVIC_ST_RELOAD_R,(uint8_t*)nullsong);
  //BMML_TrackLoadSong(&(trackHolder.tracks[2]),NVIC_ST_RELOAD_R,(uint8_t*)nullsong);
  isSong = 1;
  NVIC_ST_CURRENT_R = 0;
  EnableInterrupts();
  
	while(1);
}

//Utilized to cause delays for toggle inputs, incase a finger isn't lifted fast enough for the next read
void wait(uint32_t time){
	uint32_t i = time * 4;
	while(i > 0){
		i--;
	}
}

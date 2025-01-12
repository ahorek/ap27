/* cpuasimd.cpp --

*/

#include <arm_neon.h>
#include <cinttypes>
#include <cstdio>
#include <pthread.h>
#include <cstdlib>
#include <iostream>

#include "cpuconst.h"

char OK61[61];
char OK67[67];
char OK71[71];
char OK73[73];
char OK79[79];
char OK83[83];
char OK89[89];
char OK97[97];
char OK101[101];
char OK103[103];
char OK107[107];
char OK109[109];
char OK113[113];
char OK127[127];
char OK131[131];
char OK137[137];
char OK139[139];
char OK149[149];
char OK151[151];
char OK157[157];
char OK163[163];
char OK167[167];
char OK173[173];
char OK179[179];
char OK181[181];
char OK191[191];
char OK193[193];
char OK197[197];
char OK199[199];
char OK211[211];
char OK223[223];
char OK227[227];
char OK229[229];
char OK233[233];
char OK239[239];
char OK241[241];
char OK251[251];
char OK257[257];
char OK263[263];
char OK269[269];
char OK271[271];
char OK277[277];
char OK281[281];
char OK283[283];
char OK293[293];
char OK307[307];
char OK311[311];
char OK313[313];
char OK317[317];
char OK331[331];
char OK337[337];
char OK347[347];
char OK349[349];
char OK353[353];
char OK359[359];
char OK367[367];
char OK373[373];
char OK379[379];
char OK383[383];
char OK389[389];
char OK397[397];
char OK401[401];
char OK409[409];
char OK419[419];
char OK421[421];
char OK431[431];
char OK433[433];
char OK439[439];
char OK443[443];
char OK449[449];
char OK457[457];
char OK461[461];
char OK463[463];
char OK467[467];
char OK479[479];
char OK487[487];
char OK491[491];
char OK499[499];
char OK503[503];
char OK509[509];
char OK521[521];
char OK523[523];
char OK541[541];

uint32x4_t ixOKOK61[61];
uint32x4_t ixOKOK67[67];
uint32x4_t ixOKOK71[71];
uint32x4_t ixOKOK73[73];
uint32x4_t ixOKOK79[79];
uint32x4_t ixOKOK83[83];
uint32x4_t ixOKOK89[89];
uint32x4_t ixOKOK97[97];
uint32x4_t ixOKOK101[101];
uint32x4_t ixOKOK103[103];
uint32x4_t ixOKOK107[107];
uint32x4_t ixOKOK109[109];
uint32x4_t ixOKOK113[113];
uint32x4_t ixOKOK127[127];
uint32x4_t ixOKOK131[131];
uint32x4_t ixOKOK137[137];
uint32x4_t ixOKOK139[139];
uint32x4_t ixOKOK149[149];
uint32x4_t ixOKOK151[151];
uint32x4_t ixOKOK157[157];
uint32x4_t ixOKOK163[163];
uint32x4_t ixOKOK167[167];
uint32x4_t ixOKOK173[173];
uint32x4_t ixOKOK179[179];
uint32x4_t ixOKOK181[181];
uint32x4_t ixOKOK191[191];
uint32x4_t ixOKOK193[193];
uint32x4_t ixOKOK197[197];
uint32x4_t ixOKOK199[199];
uint32x4_t ixOKOK211[211];
uint32x4_t ixOKOK223[223];
uint32x4_t ixOKOK227[227];
uint32x4_t ixOKOK229[229];
uint32x4_t ixOKOK233[233];
uint32x4_t ixOKOK239[239];
uint32x4_t ixOKOK241[241];
uint32x4_t ixOKOK251[251];
uint32x4_t ixOKOK257[257];
uint32x4_t ixOKOK263[263];
uint32x4_t ixOKOK269[269];
uint32x4_t ixOKOK271[271];
uint32x4_t ixOKOK277[277];


// true if any element is not zero
#define continue_sito(_X) (vmaxvq_u32(_X) != 0)

#define MAKE_OK(_X) \
  for(j=0;j<_X;j++) \
    OK##_X[j]=1; \
  for(j=(_X-23);j<=_X;j++) \
    OK##_X[(j*(STEP%_X))%_X]=0;


#define MAKE_OKOK(_X) \
  for(j=0;j<_X;j++){ \
    sOKOK[0]=0; \
    sOKOK[1]=0; \
    for(jj=0;jj<64;jj++){ \
      if(SHIFT < maxshift) \
        sOKOK[0]|=(((uint64_t)OK##_X[(j+(jj+SHIFT)*MOD)%_X])<<jj); \
      if(SHIFT+64 < maxshift) \
        sOKOK[1]|=(((uint64_t)OK##_X[(j+(jj+SHIFT+64)*MOD)%_X])<<jj); \
    } \
    ixOKOK##_X[j] = vld1q_u32( (uint32_t*)sOKOK ); \
  }

void printx(uint32x4_t vec) {
    uint32_t elements[8];
    vst1q_u32(elements, vec); // Store vector elements into an array

    printf("uint32x4_t: [");
    for (int i = 0; i < 4; ++i) {
        printf("%d", elements[i]);
        if (i < 7) printf(", ");
    }
    printf("]\n");
}


void *thr_func_asimd(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    int i43, i47, i53, i59;
    uint64_t n, n43, n47, n53, n59;
    time_t boinc_last, boinc_curr;
    double cc, dd;
    uint64_t sito[2] __attribute__ ((aligned (16)));
    int16_t rems[8] __attribute__ ((aligned (16)));
    int16_t rrems[8] __attribute__ ((aligned (16)));
    const uint32x4_t ZERO128 = vdupq_n_u32(0);
    uint32_t checksum = 0;
    uint32_t apcount = 0;

    if(data->id == 0){
	time(&boinc_last);
	cc = (double)( data->K_DONE*numn43s*5 + data->iteration*numn43s );
	dd = 1.0 / (double)( data->K_COUNT*numn43s*5 );		
    }

    ckerr(pthread_mutex_lock(&lock1));
    int start = current_n43;
    int stop = start + thread_range;
    if(stop > numn43s) stop = numn43s;
    current_n43 = stop;
    ckerr(pthread_mutex_unlock(&lock1));

    while(start < numn43s){
	for(;start<stop;++start){
	    
	    if(data->id == 0){
		time (&boinc_curr);
		if( ((int)boinc_curr - (int)boinc_last) > 5 ){
		    double prog = (cc + (double)start ) * dd;
		    Progress(prog);
		    boinc_last = boinc_curr;
		}
	    }
	    
	    n43=n43_h[start];
	    for(i43=(PRIME5-24);i43>0;i43--){
		n47=n43;
		for(i47=(PRIME6-24);i47>0;i47--){
		    n53=n47;
		    for(i53=(PRIME7-24);i53>0;i53--){
			n59=n53;
			rems[0] = REM(n59,61,6);
			rems[1] = REM(n59,67,7);
			rems[2] = REM(n59,71,7);
			rems[3] = REM(n59,73,7);
			rems[4] = REM(n59,79,7);
			rems[5] = REM(n59,83,7);
			rems[6] = REM(n59,89,7);
			rems[7] = REM(n59,97,7);
			rrems[0] = REM(n59,101,7);
			rrems[1] = REM(n59,103,7);
			rrems[2] = REM(n59,107,7);
			rrems[3] = REM(n59,109,7);
			rrems[4] = REM(n59,113,7);
			rrems[5] = REM(n59,127,7);
			rrems[6] = REM(n59,131,8);
			rrems[7] = REM(n59,137,8);
			uint32x4_t r_numvec1 = vld1q_u32( (uint32_t*)rems);
			uint32x4_t r_numvec2 = vld1q_u32( (uint32_t*)rrems);
			printx(r_numvec1);
			printx(r_numvec2);

			for(i59=(PRIME8-24);i59>0;i59--){
			    
			    if(i59 < 35){
				vst1q_u32( (uint32_t*)rems, r_numvec1);
				vst1q_u32( (uint32_t*)rrems, r_numvec2);
			    }

                std::cout << rems[0] << std::endl;
				std::cout << rems[1] << std::endl;;
				printx(ixOKOK61[rems[0]]);
				printx(ixOKOK67[rems[1]]);
				uint32x4_t isito = vandq_u32( ixOKOK61[rems[0]], ixOKOK67[rems[1]] );
				printx(isito);
				exit(1);

			    
			    isito = vandq_u32( isito, ixOKOK71[rems[2]] );
			    isito = vandq_u32( isito, ixOKOK73[rems[3]] );
			    isito = vandq_u32( isito, ixOKOK79[rems[4]] );
			    isito = vandq_u32( isito, ixOKOK83[rems[5]] );
			    isito = vandq_u32( isito, ixOKOK89[rems[6]] );
			    isito = vandq_u32( isito, ixOKOK97[rems[7]] );
			    isito = vandq_u32( isito, ixOKOK101[rrems[0]] );
			    isito = vandq_u32( isito, ixOKOK103[rrems[1]] );
			    isito = vandq_u32( isito, ixOKOK107[rrems[2]] );
			    isito = vandq_u32( isito, ixOKOK109[rrems[3]] );
			    isito = vandq_u32( isito, ixOKOK113[rrems[4]] );
			    isito = vandq_u32( isito, ixOKOK127[rrems[5]] );
			    isito = vandq_u32( isito, ixOKOK131[rrems[6]] );
			    isito = vandq_u32( isito, ixOKOK137[rrems[7]] );
			    if( continue_sito(isito) ){
				isito = vandq_u32( isito, ixOKOK139[REM(n59,139,8)] );
				isito = vandq_u32( isito, ixOKOK149[REM(n59,149,8)] );
				isito = vandq_u32( isito, ixOKOK151[REM(n59,151,8)] );
				isito = vandq_u32( isito, ixOKOK157[REM(n59,157,8)] );
				isito = vandq_u32( isito, ixOKOK163[REM(n59,163,8)] );
				isito = vandq_u32( isito, ixOKOK167[REM(n59,167,8)] );
				isito = vandq_u32( isito, ixOKOK173[REM(n59,173,8)] );
				isito = vandq_u32( isito, ixOKOK179[REM(n59,179,8)] );
				isito = vandq_u32( isito, ixOKOK181[REM(n59,181,8)] );
				isito = vandq_u32( isito, ixOKOK191[REM(n59,191,8)] );
				isito = vandq_u32( isito, ixOKOK193[REM(n59,193,8)] );
				isito = vandq_u32( isito, ixOKOK197[REM(n59,197,8)] );
				isito = vandq_u32( isito, ixOKOK199[REM(n59,199,8)] );
			    if( continue_sito(isito) ){
				isito = vandq_u32( isito, ixOKOK211[REM(n59,211,8)] );
				isito = vandq_u32( isito, ixOKOK223[REM(n59,223,8)] );
				isito = vandq_u32( isito, ixOKOK227[REM(n59,227,8)] );
				isito = vandq_u32( isito, ixOKOK229[REM(n59,229,8)] );
				isito = vandq_u32( isito, ixOKOK233[REM(n59,233,8)] );
				isito = vandq_u32( isito, ixOKOK239[REM(n59,239,8)] );
				isito = vandq_u32( isito, ixOKOK241[REM(n59,241,8)] );
				isito = vandq_u32( isito, ixOKOK251[REM(n59,251,8)] );
				isito = vandq_u32( isito, ixOKOK257[REM(n59,257,9)] );
				isito = vandq_u32( isito, ixOKOK263[REM(n59,263,9)] );
				isito = vandq_u32( isito, ixOKOK269[REM(n59,269,9)] );
				isito = vandq_u32( isito, ixOKOK271[REM(n59,271,9)] );
				isito = vandq_u32( isito, ixOKOK277[REM(n59,277,9)] );
			    if( continue_sito(isito) ){
				vst1q_u32( (uint32_t*)sito, isito );
				for(int ii=0;ii<2;++ii){
				    while(sito[ii]){
					int setbit = 63 - __builtin_clzll(sito[ii]);
					uint64_t n = n59+( setbit + data->SHIFT + (64*ii) )*MOD;

					if(n%7)
					if(n%11)
					if(n%13)
					if(n%17)
					if(n%19)
					if(n%23)
					if(OK281[n%281])
					if(OK283[n%283])
					if(OK293[n%293])
					if(OK307[n%307])
					if(OK311[n%311])
					if(OK313[n%313])
					if(OK317[n%317])
					if(OK331[n%331])
					if(OK337[n%337])
					if(OK347[n%347])
					if(OK349[n%349])
					if(OK353[n%353])
					if(OK359[n%359])
					if(OK367[n%367])
					if(OK373[n%373])
					if(OK379[n%379])
					if(OK383[n%383])
					if(OK389[n%389])
					if(OK397[n%397])
					if(OK401[n%401])
					if(OK409[n%409])
					if(OK419[n%419])
					if(OK421[n%421])
					if(OK431[n%431])
					if(OK433[n%433])
					if(OK439[n%439])
					if(OK443[n%443])
					if(OK449[n%449])
					if(OK457[n%457])
					if(OK461[n%461])
					if(OK463[n%463])
					if(OK467[n%467])
					if(OK479[n%479])
					if(OK487[n%487])
					if(OK491[n%491])
					if(OK499[n%499])
					if(OK503[n%503])
					if(OK509[n%509])
					if(OK521[n%521])
					if(OK523[n%523])
					if(OK541[n%541]){
					    int k = 0;
					    uint64_t m = n + data->STEP * 5;
					    while(PrimeQ(m)){
						k++;
						m += data->STEP;
					    }
					    
					    if(k>=10){
						m = n + data->STEP * 4;
						uint64_t mstart = m;
						while(PrimeQ(m)){
						    k++;
						    m -= data->STEP;
						    if(m > mstart) break;
						}
					    }

					    if(k>=10){
						uint64_t first_term = m + data->STEP;

						ReportSolution(k, data->K, first_term, checksum);
						++apcount;
					    }
					}
												
					sito[ii] ^= ((uint64_t)1) << setbit; // toggle bit off
				    }
				}
			    }}}

			    n59 += data->S59;

			    r_numvec1 = vaddq_u32(r_numvec1, svec1);
			    r_numvec2 = vaddq_u32(r_numvec2, svec2);

			    if(n59>=MOD){
				n59-=MOD;

				r_numvec1 = vsubq_u32(r_numvec1, mvec1);
				uint32x4_t addvec = vaddq_u32(r_numvec1, numvec1_1);
				r_numvec1 = vbslq_u32(vcgtq_u32( ZERO128, r_numvec1), r_numvec1, addvec );
				

				r_numvec2 = vsubq_u32(r_numvec2, mvec2);
				addvec = vaddq_u32(r_numvec2, numvec1_2);
				r_numvec2 = vbslq_u32(vcgtq_u32( ZERO128, r_numvec2 ), r_numvec2, addvec );
			    }
			    uint32x4_t subvec = vsubq_u32(r_numvec1, numvec1_1);
			    r_numvec1 = vbslq_u32(vcgtq_u32(r_numvec1, numvec2_1), r_numvec1, subvec );

			    subvec = vsubq_u32(r_numvec2, numvec1_2);
			    r_numvec2 = vbslq_u32(vcgtq_u32(r_numvec2, numvec2_2), r_numvec2, subvec );						
			   
			}     
			n53 += data->S53;
			if(n53>=MOD)n53-=MOD;
		    }
		    n47 += data->S47;
		    if(n47>=MOD)n47-=MOD;
		}
		n43 += data->S43;
		if(n43>=MOD)n43-=MOD;
	    }
	}

	ckerr(pthread_mutex_lock(&lock1));
	start = current_n43;
	stop = start + thread_range;
	if(stop > numn43s) stop = numn43s;
	current_n43 = stop;
	ckerr(pthread_mutex_unlock(&lock1));
    }
    
    // add this threads checksum and ap count to total
    ckerr(pthread_mutex_lock(&lock3));
    uint64_t total = cksum;
    total += checksum;
    if(total > MAXINTV){
	total -= MAXINTV;
    }
    cksum = total;
    totalaps += apcount;
    ckerr(pthread_mutex_unlock(&lock3));		

    pthread_exit(NULL);

    return NULL;
}


void Search_asimd(int K, int startSHIFT, int K_COUNT, int K_DONE, int threads)
{ 
    int i3, i5, i31, i37, i41;
    int SHIFT;
    int maxshift = startSHIFT+640;
    uint64_t STEP;
    uint64_t n0;
    uint64_t S31, S37, S41, S43, S47, S53, S59;
    int j,jj,k;
    int err;
    uint64_t sOKOK[2] __attribute__ ((aligned (16)));

    time_t start_time, finish_time;

    if(boinc_standalone()){
	time (&start_time);
    }

    STEP=K*PRIM23;
    n0=(N0*(K%17835)+((N0*17835)%MOD)*(K/17835)+N30)%MOD;

    S31=(PRES2*(K%17835)+((PRES2*17835)%MOD)*(K/17835))%MOD;
    S37=(PRES3*(K%17835)+((PRES3*17835)%MOD)*(K/17835))%MOD;
    S41=(PRES4*(K%17835)+((PRES4*17835)%MOD)*(K/17835))%MOD;
    S43=(PRES5*(K%17835)+((PRES5*17835)%MOD)*(K/17835))%MOD;
    S47=(PRES6*(K%17835)+((PRES6*17835)%MOD)*(K/17835))%MOD;
    S53=(PRES7*(K%17835)+((PRES7*17835)%MOD)*(K/17835))%MOD;
    S59=(PRES8*(K%17835)+((PRES8*17835)%MOD)*(K/17835))%MOD;

    int count=0;

    for(i31=0;i31<7;++i31)
    for(i37=0;i37<13;++i37)
    if(i37-i31<=10&&i31-i37<=4)
    for(i41=0;i41<17;++i41)
    if(i41-i31<=14&&i41-i37<=14&&i31-i41<=4&&i37-i41<=10)
    for(i3=0;i3<2;++i3)
    for(i5=0;i5<4;++i5){ 
	n43_h[count]=(n0+i3*S3+i5*S5+i31*S31+i37*S37+i41*S41)%MOD;  //10840 of these  12673 n53 per
	count++;
    }

    //quick loop vectors
    int16_t s1arr[8] __attribute__ ((aligned (16))) = { (int16_t)(S59%61), (int16_t)(S59%67), (int16_t)(S59%71), (int16_t)(S59%73), (int16_t)(S59%79), (int16_t)(S59%83), (int16_t)(S59%89), (int16_t)(S59%97) };
    svec1 = vld1q_u32( (uint32_t*)s1arr);

    int16_t s2arr[8] __attribute__ ((aligned (16))) = { (int16_t)(S59%101), (int16_t)(S59%103), (int16_t)(S59%107), (int16_t)(S59%109), (int16_t)(S59%113), (int16_t)(S59%127), (int16_t)(S59%131), (int16_t)(S59%137) };
    svec2 = vld1q_u32( (uint32_t*)s2arr);

    int16_t m1arr[8] __attribute__ ((aligned (16))) = { (int16_t)(MOD%61), (int16_t)(MOD%67), (int16_t)(MOD%71), (int16_t)(MOD%73), (int16_t)(MOD%79), (int16_t)(MOD%83), (int16_t)(MOD%89), (int16_t)(MOD%97) };
    mvec1 = vld1q_u32( (uint32_t*)m1arr);

    int16_t m2arr[8] __attribute__ ((aligned (16))) = { (int16_t)(MOD%101), (int16_t)(MOD%103), (int16_t)(MOD%107), (int16_t)(MOD%109), (int16_t)(MOD%113), (int16_t)(MOD%127), (int16_t)(MOD%131), (int16_t)(MOD%137) };
    mvec2 = vld1q_u32( (uint32_t*)m2arr);

    int16_t nv11arr[8] __attribute__ ((aligned (16))) = { 61, 67, 71, 73, 79, 83, 89, 97 };
    numvec1_1 = vld1q_u32( (uint32_t*)nv11arr);

    int16_t nv21arr[8] __attribute__ ((aligned (16))) = { 60, 66, 70, 72, 78, 82, 88, 96 };
    numvec2_1 = vld1q_u32( (uint32_t*)nv21arr);
    
    int16_t nv12arr[8] __attribute__ ((aligned (16))) = { 101, 103, 107, 109, 113, 127, 131, 137 };
    numvec1_2 = vld1q_u32( (uint32_t*)nv12arr);

    int16_t nv22arr[8] __attribute__ ((aligned (16))) = { 100, 102, 106, 108, 112, 126, 130, 136 };
    numvec2_2 = vld1q_u32( (uint32_t*)nv22arr);

    // init OK arrays    
    MAKE_OK(61);
    MAKE_OK(67);
    MAKE_OK(71);
    MAKE_OK(73);
    MAKE_OK(79);
    MAKE_OK(83);
    MAKE_OK(89);
    MAKE_OK(97);
    MAKE_OK(101);
    MAKE_OK(103);
    MAKE_OK(107);
    MAKE_OK(109);
    MAKE_OK(113);
    MAKE_OK(127);
    MAKE_OK(131);
    MAKE_OK(137);
    MAKE_OK(139);
    MAKE_OK(149);
    MAKE_OK(151);
    MAKE_OK(157);
    MAKE_OK(163);
    MAKE_OK(167);
    MAKE_OK(173);
    MAKE_OK(179);
    MAKE_OK(181);
    MAKE_OK(191);
    MAKE_OK(193);
    MAKE_OK(197);
    MAKE_OK(199);
    MAKE_OK(211);
    MAKE_OK(223);
    MAKE_OK(227);
    MAKE_OK(229);
    MAKE_OK(233);
    MAKE_OK(239);
    MAKE_OK(241);
    MAKE_OK(251);
    MAKE_OK(257);
    MAKE_OK(263);
    MAKE_OK(269);
    MAKE_OK(271);
    MAKE_OK(277);
    MAKE_OK(281);
    MAKE_OK(283);
    MAKE_OK(293);
    MAKE_OK(307);
    MAKE_OK(311);
    MAKE_OK(313);
    MAKE_OK(317);
    MAKE_OK(331);
    MAKE_OK(337);
    MAKE_OK(347);
    MAKE_OK(349);
    MAKE_OK(353);
    MAKE_OK(359);
    MAKE_OK(367);
    MAKE_OK(373);
    MAKE_OK(379);
    MAKE_OK(383);
    MAKE_OK(389);
    MAKE_OK(397);
    MAKE_OK(401);
    MAKE_OK(409);
    MAKE_OK(419);
    MAKE_OK(421);
    MAKE_OK(431);
    MAKE_OK(433);
    MAKE_OK(439);
    MAKE_OK(443);
    MAKE_OK(449);
    MAKE_OK(457);
    MAKE_OK(461);
    MAKE_OK(463);
    MAKE_OK(467);
    MAKE_OK(479);
    MAKE_OK(487);
    MAKE_OK(491);
    MAKE_OK(499);
    MAKE_OK(503);
    MAKE_OK(509);
    MAKE_OK(521);
    MAKE_OK(523);
    MAKE_OK(541);
	
    int iteration = 0;

    // 10 shift
    for(SHIFT=startSHIFT; SHIFT<maxshift; SHIFT+=128){

	MAKE_OKOK(61);
	MAKE_OKOK(67);
	MAKE_OKOK(71);
	MAKE_OKOK(73);
	MAKE_OKOK(79);
	MAKE_OKOK(83);
	MAKE_OKOK(89);
	MAKE_OKOK(97);
	MAKE_OKOK(101);
	MAKE_OKOK(103);
	MAKE_OKOK(107);
	MAKE_OKOK(109);
	MAKE_OKOK(113);
	MAKE_OKOK(127);
	MAKE_OKOK(131);
	MAKE_OKOK(137);
	MAKE_OKOK(139);
	MAKE_OKOK(149);
	MAKE_OKOK(151);
	MAKE_OKOK(157);
	MAKE_OKOK(163);
	MAKE_OKOK(167);
	MAKE_OKOK(173);
	MAKE_OKOK(179);
	MAKE_OKOK(181);
	MAKE_OKOK(191);
	MAKE_OKOK(193);
	MAKE_OKOK(197);
	MAKE_OKOK(199);
	MAKE_OKOK(211);
	MAKE_OKOK(223);
	MAKE_OKOK(227);
	MAKE_OKOK(229);
	MAKE_OKOK(233);
	MAKE_OKOK(239);
	MAKE_OKOK(241);
	MAKE_OKOK(251);
	MAKE_OKOK(257);
	MAKE_OKOK(263);
	MAKE_OKOK(269);
	MAKE_OKOK(271);
	MAKE_OKOK(277);

	pthread_t thr[threads];

	// create a thread_data_t argument array
	thread_data_t thr_data[threads];

	// initialize shared data
	current_n43 = 0;

	// create threads
	for (k = 0; k < threads; ++k) {
	    thr_data[k].id = k;
	    thr_data[k].K = K;
	    thr_data[k].K_COUNT = K_COUNT;
	    thr_data[k].K_DONE = K_DONE;
	    thr_data[k].SHIFT = SHIFT;
	    thr_data[k].STEP = STEP;
	    thr_data[k].S43 = S43;
	    thr_data[k].S47 = S47;
	    thr_data[k].S53 = S53;
	    thr_data[k].S59 = S59;
	    thr_data[k].iteration = iteration;
	    err = pthread_create(&thr[k], NULL, thr_func_asimd, &thr_data[k]);
	    if (err){
		fprintf(stderr, "ERROR: pthread_create, code: %d\n", err);
		exit(EXIT_FAILURE);
	    }
	}

	// block until all threads complete
	for (k = 0; k < threads; ++k) {
	    err = pthread_join(thr[k], NULL);
	    if (err){
		fprintf(stderr, "ERROR: pthread_join, code: %d\n", err);
		exit(EXIT_FAILURE);
	    }
	}

	++iteration;
	
    }

    if(boinc_standalone()){
	time(&finish_time);
	printf("Computation of K: %d complete in %d seconds\n", K, (int)finish_time - (int)start_time);
    }


}

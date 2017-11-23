/******************************************************/
/***This file is used to simulated in systemc-ams. ****/
/***                  Yukai Chen                   ****/
/***                     EDA                       ****/
/******************************************************/



#include "testbench.h"



void testbench::set_attributes() 
{
	 CORE_p.set_timestep(1,sc_core::SC_MS); 
	 MEM_p.set_timestep(1,sc_core::SC_MS); 
	 RFTR_p.set_timestep(1,sc_core::SC_MS); 
	 UART_p.set_timestep(1,sc_core::SC_MS); 


	 CORE_p.set_delay(1); 
	 MEM_p.set_delay(1); 
	 RFTR_p.set_delay(1); 
	 UART_p.set_delay(1); 
}


void testbench::initialize()
{
	 int j;

	 ifstream in("../powertrace.txt");
	 if(!in) {
		 cout<<"Cannot open file."<<endl;
	 exit(-1);
	 }
	 cout<<"Power value read..."<<endl;
	 for(j=0;j<NUM;j++) {
	 in  >> CORE_val[j] >> MEM_val[j] >> RFTR_val[j] >> UART_val[j];
	}
	 in.close();


	 mytempfile.open("../temptrace.txt");


}

void testbench::processing()
{
	 int i;
 	 sum_CORE =0;
 	 sum_MEM =0;
 	 sum_RFTR =0;
 	 sum_UART =0;


	 for(i=0;i<NUM;i++){
 		 sum_CORE +=CORE_val[i];
 		 sum_MEM +=MEM_val[i];
 		 sum_RFTR +=RFTR_val[i];
 		 sum_UART +=UART_val[i];
	 }


 	 avg_CORE = sum_CORE/NUM;
 	 avg_MEM = sum_MEM/NUM;
 	 avg_RFTR = sum_RFTR/NUM;
 	 avg_UART = sum_UART/NUM;


 	 CORE_p.write(avg_CORE);
 	 MEM_p.write(avg_MEM);
 	 RFTR_p.write(avg_RFTR);
 	 UART_p.write(avg_UART);


 	 temp_CORE = CORE_t.read()+313.15;
 	 temp_MEM = MEM_t.read()+313.15;
 	 temp_RFTR = RFTR_t.read()+313.15;
 	 temp_UART = UART_t.read()+313.15;


 	 mytempfile <<std::setprecision(10)<< temp_CORE <<"\t";
 	 mytempfile <<std::setprecision(10)<< temp_MEM <<"\t";
 	 mytempfile <<std::setprecision(10)<< temp_RFTR <<"\t";
 	 mytempfile <<std::setprecision(10)<< temp_UART <<"\n";

	//counter++;

       // if(counter == 2)
       // sc_stop();
}



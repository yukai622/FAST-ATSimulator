/******************************************************/
/***This file is used to simulated in systemc-ams. ****/
/***                  Yukai Chen                   ****/
/***                     EDA                       ****/
/******************************************************/



#include "systemc-ams.h"



#define NUM 6000

SCA_TDF_MODULE (testbench)
{
		 //Interface declaration: output
		 sca_tdf::sca_out<double> CORE_p; // Power of the CORE 
		 sca_tdf::sca_out<double> MEM_p; // Power of the MEM 
		 sca_tdf::sca_out<double> RFTR_p; // Power of the RFTR 
		 sca_tdf::sca_out<double> UART_p; // Power of the UART 


		 //Interface declaration: input
		 sca_tdf::sca_in<double> CORE_t; // Power of the CORE 
		 sca_tdf::sca_in<double> MEM_t; // Power of the MEM 
		 sca_tdf::sca_in<double> RFTR_t; // Power of the RFTR 
		 sca_tdf::sca_in<double> UART_t; // Power of the UART 


		 //Vectors to store the value read from .pptrace file
		 double CORE_val[NUM]; 
		 double MEM_val[NUM]; 
		 double RFTR_val[NUM]; 
		 double UART_val[NUM]; 


		 //Store sum 
		 double sum_CORE; 
		 double sum_MEM; 
		 double sum_RFTR; 
		 double sum_UART; 


		 //Store avg 
		 double avg_CORE; 
		 double avg_MEM; 
		 double avg_RFTR; 
		 double avg_UART; 


		 //Store value from thermal model 
		 double temp_CORE; 
		 double temp_MEM; 
		 double temp_RFTR; 
		 double temp_UART; 
		 ofstream mytempfile;

		 int counter;

		 SCA_CTOR(testbench):  CORE_p("CORE_p"),  MEM_p("MEM_p"),  RFTR_p("RFTR_p"),  UART_p("UART_p"),  counter(0) {
	 } 


		 void set_attributes(); 

		 void initialize(); 

		 void processing(); 


};


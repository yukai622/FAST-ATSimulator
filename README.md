## FAST-ATSimulator

### Fast Architectural Thermal Simulator

Temperature is a critical property of smart systems, due to its impact on reliability and to its inter-dependence
with power consumption. Unfortunately, the current design flows evaluate thermal evolution ex-post, on offline
power traces. This does not allow to consider temperature as a dimension in the design loop, and it misses all the
complex inter-dependencies with design choices and power evolution. This Fast-ATSimulator proposes to enable thermal
runtime simulation, by adopting the functional language SystemC-AMS. The system thermal model is built by
using state-of-the-art circuit equivalent models, by exploiting the support for electrical linear networks intrinsic
of SystemC-AMS. Our using experience shows that the choice of SystemC-AMS is a winning strategy
for building a simultaneous simulation of multiple aspects of a smart system. The generated code exposes
a high degree of accuracy with respect to the reference thermal simulator HotSpot. 

#### The codes in this poject consist of the following phases:


![1.png](https://steemitimages.com/DQmRSHNr3yikNfUUNo5LKptdMTYdYXP49w5Cit5arseT7gh/1.png)

1.**Construction of the RC network**, by reproducing the method used by HotSpot using chip floorplan and technology data;

2. **Please check the detailed implementation in our paper published in ACM Transactions on Design Automation of Electronic Systems (TODAES).** 
   -Click here go to ACM digital library: [SystemC-AMS Thermal Modeling for the Co-simulation of Functional and Extra-Functional Properties](https://dl.acm.org/citation.cfm?id=3267125)
   -If you don't have permission enter ACM digital library, click [here](https://www.researchgate.net/publication/329851294_SystemC-AMS_Thermal_Modeling_for_the_Co-simulation_of_Functional_and_Extra-Functional_Properties) 




#### Please reference below paper if you use this tool to run your thermal simualtion or build thermal model:

>Chen, Yukai, Sara Vinco, Enrico Macii, and Massimo Poncino. "SystemC-AMS Thermal Modeling for the Co-simulation of Functional and Extra-Functional Properties." ACM Transactions on Design Automation of Electronic Systems (TODAES) 24, no. 1 (2018): 4.


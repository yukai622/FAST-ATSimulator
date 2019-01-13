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

2.Coming soon...





#### Please reference any paper we list here if you use this tool to run your thermal simualtion or build thermal model:

>Chen, Y., Vinco, S., Macii, E. and Poncino, M., 2016, May. Fast thermal simulation using SystemC-AMS. In Great Lakes Symposium on VLSI, 2016 International (pp. 427-432). IEEE.


>Chen, Y., Vinco, S., Macii, E. and Poncino, M., 2018. SystemC-AMS Thermal Modeling for the Co-simulation of Functional and Extra-Functional Properties. ACM Transactions on Design Automation of Electronic Systems (TODAES), 24(1), p.4.


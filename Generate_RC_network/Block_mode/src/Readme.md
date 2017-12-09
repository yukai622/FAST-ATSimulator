**Remember to modify the values of technology physical parameters in your design.**

e.g. *chip thickness in meters*
     *chip thermal conductivity in W/(m-K)* 
     *chip specific heat in J/(m^3-K)*

All these parameters are defined in the **temperature.cpp** file:

>/* default thermal configuration parameters	*/
>thermal_config_t default_thermal_config(void)

**Remember to modify the path indicats your floorplan file.**

In the **rc_network.h** file:

>//Read the floorplan file indicates the layout of each component of the system
>static char *flp_file = "../example.flp" ; //Change your floorplan file in here



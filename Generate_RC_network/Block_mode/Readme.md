## Generate thermal resistor and capacitor networks according to floorplan and technology parameters

### Input files
1. **The floorplan of your design**. Please refer the example floorplan format in this folder.
2. **The technology paratemters**. Please refer the parameter list in the file teperature.cpp uner /src foler. lease check the method *thermal_config_t default_thermal_config(void)*  

### Output file

The generated thermal resistor and capacitor network, the thermal model, will be used in the thermal simulator. Please see the r&c network example file in this folder.

### The example thermal resistor and capacitor network in the SystemC-AMS format:

![3.png](https://steemitimages.com/DQmcwBahi75RNLFyMFyLeLWoct12MzhUG1ZaTExBgnExANw/3.png)

**Interface Modeling:** The electrical circuit equivalent model is instantiated as a single SystemC module (SC MODULE, line 1) that encapsulates the entire RC network. The interface of the module consists of two ports for each component: one input port is used to collect the evolution of power consumption over time, whereas the output port conveys the corresponding temperature.
The flexibility of SystemC-AMS allows to decouple the semantics of the interface from the semantics of the actual behavior implementation. For this reason, the abstraction level adopted for ports is TDF, that determines a fixed timestep at which input ports are read, RC network is evaluated and output ports are updated.

**Module Implementation:** The body of the SC MODULE includes the implementation of the RC network. This is achieved by adopting the ELN level of abstraction, which natively supports electrical network elements. The construction of the SystemC-AMS thermal model is thus a one-to-one mapping of circuit elements into SystemC-AMS primitives. Each circuit node is implemented as a SystemC-AMS ELN node (sca_node, lines 6–7), but for the ground element, which is represented with an ad-hoc primitive (sca node ref, line 5). 
Resistors are mapped to instances of the sca_r primitive, which represents SystemC-AMS resistors(line 12). 
Capacitors are mapped to an instance of the sca_c primitive, representing SystemC-AMS capacitors (line 11).
The input power ports are connected to the ELN circuit via current source primitives, that transform a numerical value into a current (sca_isource primitive, lines 9 and 15–18).
Voltage sinks are represented with instances of the sca_vsink primitive, whose output terminal is connected to the corresponding output temperature port (the black square terminals, lines 10 and 19–22).


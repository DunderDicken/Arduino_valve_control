#ifndef _SWITCHABLE_H_
#define _SWITCHABLE_H_

//Base class for output that can be switched on/off via single digital pin:
class Switchable  
{
	public:

		// Consturcutor accepts pin number for output
	    Switchable(const int pin);
		
		// Turn pin on
		void on();
	    
	    // Turn pin off
		void off();
		
		// Toggle pin
		void toggle();
		
		// dim pin
		void dim(int dimVal);
		
		// Get current state
		bool getState();
		
		// Set state with bool variable
		void setState(bool state);

    // Get latest on-time duration
    unsigned long getLastOnTime();
	
	private:
	
		const int m_pin; 	//output pin
		bool m_state;		//current state
    unsigned long m_lastOnTime; // keep track of on-time
    unsigned long m_startTime;
};

#endif // _SWITCHABLE_H_
 

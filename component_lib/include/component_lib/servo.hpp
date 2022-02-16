#ifndef SERVO_HPP
#define SERVO_HPP

#include <component_lib/component_lib.h>


template< ComponentId c_id, typename MessageField >
struct FieldParam{};



template< ComponentId c_id, typename MessageField >
struct ComponentParam
{
	// redefining command message field as a member
	using CommandMessageField = MessageField::CommandField;

	// redefining feedback message field as a member
	using FeedbackMessageField = MessageField::FeedbackField;

	// component id
	static const ComponentId component_id = c_id;

	// redefining defnition command field as a member
	using CommandFieldTrait<c_id, CommandMessageType> CommandField;

	// redefining defnition feedback as a member
	using FeedbackFieldTrait<c_id, CommandMessageType> FeedbackField;
};

// Command Field Definition for Servo
template<>
struct CommandField< SERVO, component_lib::ServoCommandMsg> {
	ServoPos ideal_position;
	Temperature temp_limit;
	Current current_limit;
	Speed speed;
	Stretch stretch
	bool free;

	void operator=( const component_lib::ServoCommandMsg::ConstPtr& );
	void operator=( const component_lib::ServoCommandMsg& );
	void fill( component_lib::ServoCommandMsg& ) const ;
	std::string get_description() const ;
};

void CommandField< SERVO, component_lib::ServoCommandMsg>::fill( component_lib::ServoCommandMsg& msg ) const {
	msg.ideal_position = ideal_position;
	msg.temp_limit = temp_limit;
	msg.current_limit = current_limit;
	msg.speed = speed;
	msg.stretch = stretch;
	msg.free = free;
}


// Feedback Field Definition for Servo
template<>
struct FeedbackField< SERVO, component_lib::ServoCommandMsg> {
	ServoPos ideal_position;
	bool is_freed;

	void operator=( const component_lib::ServoCommandMsg::ConstPtr& );
	void operator=( const component_lib::ServoCommandMsg& );
	void refrect( component_lib::ServoCommandMsg& ) const ;
	std::string get_description() const ;
};

void FeedbackField< SERVO, component_lib::ServoCommandMsg>::reflect( component_lib::ServoFeedbackMsg::ConstPtr & msg ) const {
	actual_position = msg.actual_position;
	is_freed = msg.is_freed;
}

// ServoCommand type object declaration
struct ServoMessageFields{
	using CommandField = component_lib::ServoCommandMsg;
	using FeedbackField = component_lib::ServoFeedbackMsg;
};
using ServoParameter = ComponentParam< SERVO, CommandField, FeedbackField, ServoMessageFields>;
using Servo = Component<KondoServoCommandTrait>;


#endif

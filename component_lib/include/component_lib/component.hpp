#ifndef COMPONENT_H
#define COMPONENT_H

#include <rclcpp/rclcpp.hpp>
#include <string>
#include <component_lib/component_msgs.h>

// The default Component class where component_state != NONE;
template< typename ComponentParam >
class Component{
	public:
    // Command Field Type
    using CommandField = typename ComponenParam::CommandField;

    // Feedback Feedback Type
    using FeedbackField = typename ComponentParam::FeedbackField;

    // Command Message Type
		using CommandMessageField = typename ComponentParam::CommandMessage;

    // Feedback Message Type
    using FeedbackMessageField = typename ComponentParam::FeeedbackMessage;


		ID id;

		// The default (and only implementation of) MessageHandler class where s = COMMAND || FEEDBACK || GENERAL;
		template< typename MessageT >
		class MessageHandler {
			public:
				neko_util::CattyError check_msg_id( Component* parent_ptr, const typename MessageT::ConstPtr& msg_ptr ) const {
					if( parent_ptr->id != msg_ptr->id ) return ID_NOT_MATCH;
					else return SUCCESS;
				}

				wb::Error check_msg_id( Component* parent_ptr, MessageT& msg ) const {
					if( parent_ptr->id != msg.id ) return ID_NOT_MATCH;
					else return SUCCESS;
				}

			 	wb::Error set_msg( Component* parent_ptr, CommandMessageField & msg ) const {
					parent_ptr->id.fill( msg.id );
					parent_ptr->command.fill( msg );
					return SUCCESS;
				}

				wb::Error reflect_msg( Component* parent_ptr, const typename MessageT::ConstPtr& msg_ptr ) {
					wb::Error error = this->check_msg_id( parent_ptr, msg_ptr );
					if( error ) return error;
					parent_ptr->feedback = msg_ptr;
					return SUCCESS;
				}
		};

	private:
		// global id
		GlobalId id;

		// message type
		Message msg;

		// messenge handler
		MessageHandler< typename ComponentParam::MessageField> message_handler;

		// command field
		CommandField command;

		// feedback field
		FeedbackField feedback;

		// bounds
		typename ComponentTraitType::Bound bounds;

		// parameters for command spaces generation
		typename ComponentTraitType::ParametersForCommandSpaceGeneration parameters_for_command_space_generation;


	public:

		// public interfaces
		Component( const typename ComponentTraitType::Field& field, neko_util::PartId, neko_util::IdNumber );
		Component( neko_util::PartId, neko_util::IdNumber );
		std::string info() const override;
		wb::Id get_component_id() const override ;
		GlobalId get_id() const;
		void check_id( const Component& ) const ;
		void check_id( const std::shared_ptr<Component>& ) const ;
		void print_id_error( neko_util::CattyError error, const Component& );

		typename ComponentTraitType::Field get_field() const ;
		void set_field( const typename ComponentTraitType::Field );

		void reflect_msg( const typename ComponentTraitType::Message::ConstPtr & msg_ptr ) {
			return message_handler.reflect_msg( this, msg_ptr );
		}

		void set_msg( typename ComponentTraitType::Message& msg ){
			return message_handler.set_msg( this, msg );
		}

};


#include <component_lib/component_impl.h>

#endif

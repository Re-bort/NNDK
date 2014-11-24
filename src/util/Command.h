#ifndef Command_h
#define Command_h

#include <vector>
#include <string>
#include <sstream>

#define TOKEN std::string
#define TOKENS std::vector<std::string>

class Command
{
	public:
		typedef TOKENS::iterator iterator;
		typedef TOKENS::const_iterator const_iterator;
								
								Command(unsigned argc, char ** argv);
		
		const TOKEN&			cmd(void) const;		
		const TOKENS&	        args(void) const;	
		const TOKEN& 			arg(const unsigned i) const;	
		const unsigned		    size() const;
        const TOKEN             arg_str(void) const;
				
		const_iterator 			find(const TOKEN& token) const;
		const_iterator			begin() const;
		const_iterator    		end() const;
		
        template <class T> 
        const unsigned          get(const TOKEN& token, T& value) const;
		
        template <class T> 
        const unsigned          get(const unsigned i, T& value) const;
	
	private:
		TOKEN 					command_;
		TOKENS			        arguments_;
}; 

Command::Command(unsigned argc, char ** argv)
	: command_(argv[0])
{
	for(unsigned i = 1; i < argc; ++i)
		arguments_.push_back(argv[i]);
}

inline const TOKEN& Command::cmd() const
{
	return command_;
}

inline const TOKENS& Command::args() const
{
	return arguments_;
}

inline const TOKEN Command::arg_str() const
{
    TOKEN arg_str(command_);
    if(size() > 0)
        for(unsigned i = 0; i < size(); ++i)
            arg_str += TOKEN(" ") + arguments_[i];
    return arg_str;
}
    

inline const TOKEN& Command::arg(const unsigned i) const
{
	return arguments_[i];
}

inline const unsigned Command::size() const
{
	return arguments_.size();
}

Command::const_iterator Command::find(const TOKEN& token) const
{
	const_iterator pos;
	for(pos = arguments_.begin(); pos != arguments_.end(); ++pos)
		if(pos->find(token, 0) == 0)
			break;
	return pos;
}

inline Command::const_iterator Command::begin() const
{
	return arguments_.begin();
}

inline Command::const_iterator Command::end() const
{
	return arguments_.end();
}

template <class T>
const unsigned Command::get(const TOKEN& token, T& value) const
{
	const_iterator pos = find(token);
    unsigned flag;
    if(pos != arguments_.end())
	{
        std::stringstream buffer;
        buffer << pos->substr(token.length(), pos->length());
		buffer >> value;
        
        if(buffer.fail() || buffer.bad()) 
            flag = 0;
        else flag = 1;
        
	} else flag = 0;
    
    return flag;
}

template <class T>
const unsigned Command::get(const unsigned i, T& value) const
{
	std::stringstream buffer;
	buffer << arg(i);
	buffer >> value;            
		
	unsigned flag;
    if (buffer.fail() || buffer.bad())
		flag = 0;
	else flag = 1;
        
    return flag;
}


#endif // Command_h

VERTEX_BEGIN								
#version 330								
											
uniform mat4 u_PVM;						
uniform mat4 u_M;                          
											
layout( location = 0 ) in vec4 i_Position;	
layout ( location = 5 ) in vec4 i_Colour;	
											
out vec4 o_Colour;							
											
void main()								
{											
	o_Colour = i_Colour;					
											
	gl_Position = u_PVM * i_Position;		
}											
VERTEX_END									
											
FRAGMENT_BEGIN								
#version 330								
											
in vec4 o_Colour;							
											
out vec4 FragColour;						
											
void main()								
{											
	FragColour = o_Colour;                  
}											
FRAGMENT_END                               
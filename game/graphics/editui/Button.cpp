#include "Button.h"
#include "TextureDeserializer.h"
#include "Model.h"

Button::Button(IListener<Button> &listener)
: on(true)
, tOn(*TextureDeserializer("greendot1").deserialize())
, tOff(*TextureDeserializer("greendot2").deserialize())
, quad(Model::Square())
, listener(listener)
{

}

void Button::Draw(ShaderProgram &)
{

}

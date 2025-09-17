//////////////////////////////////////////////////////////////////////////////////
//                                                                              //
// StyleAsCode exporter v2.0 - Style data exported as a values array            //
//                                                                              //
// USAGE: On init call: GuiLoadStyleLight();                                   //
//                                                                              //
// more info and bugs-report:  github.com/raysan5/raygui                        //
// feedback and support:       ray[at]raylibtech.com                            //
//                                                                              //
// Copyright (c) 2020-2025 raylib technologies (@raylibtech)                    //
//                                                                              //
//////////////////////////////////////////////////////////////////////////////////

#define LIGHT_STYLE_PROPS_COUNT  2

// Custom style name: Light
static const GuiStyleProp lightStyleProps[LIGHT_STYLE_PROPS_COUNT] = {
    { 0, 16, (int)0x00000018 },    // DEFAULT_TEXT_SIZE 
    { 0, 20, (int)0x00000024 },    // DEFAULT_TEXT_LINE_SPACING 
};

// Style loading function: Light
static void GuiLoadStyleLight(void)
{
    // Load style properties provided
    // NOTE: Default properties are propagated
    for (int i = 0; i < LIGHT_STYLE_PROPS_COUNT; i++)
    {
        GuiSetStyle(lightStyleProps[i].controlId, lightStyleProps[i].propertyId, lightStyleProps[i].propertyValue);
    }

    //-----------------------------------------------------------------

    // TODO: Custom user style setup: Set specific properties here (if required)
    // i.e. Controls specific BORDER_WIDTH, TEXT_PADDING, TEXT_ALIGNMENT
}

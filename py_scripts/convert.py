import re

# Define the replacement pattern
pattern = r'lua_pushtablestringstring\(L, "(.*?)", (\w+?)\);'

# Replacement function
def replace(match):
    event_name = match.group(1)
    constant_name = match.group(2)
    new_event_name = event_name.replace("EVENT_ADDPAYMENTINFO", constant_name.upper())
    new_event_name = event_name.replace("PARAM_ACHIEVEMENTID", constant_name.upper())
    new_event_name = event_name.replace("PROP_ACHIEVEMENTID", constant_name.upper())
    new_event_name = new_event_name.replace("KEVENT", "EVENT_")
    new_event_name = new_event_name.replace("KUSERPROPERTY", "PROP_")
    return f'lua_pushtablestringstring(L, "{new_event_name}", {constant_name});'

# Input multiline string
input_string = """
    lua_pushtablestringstring(L, "PROP_ACHIEVEMENTID", kUserPropertyAllowAdPersonalizationSignals);
    lua_pushtablestringstring(L, "PROP_ACHIEVEMENTID", kUserPropertySignUpMethod);
"""

# Perform the conversion using regular expressions
converted_string = re.sub(pattern, replace, input_string)
print(converted_string)

-- import cash lua module
local cash = require("cash")
local _ = cash._ -- utility export

-- use bash interpreter (for whatever reason... 🤮)
cash.bash("$PATH=':$PATH'")

-- set prompt (colours automatically used if possible)
cash.prompt('\[\e[38;5;76m\]\u@\h\[\e[97m\]:\[\e[38;5;69m\]\W\[\e[0m\]\\$ ') -- using PS1 string
cash.prompt({ cash.user, "@", cash.host, ">" })                              -- plain format (colours tbi)

_("PATH") = "/etc/some/bin/".._("PATH")

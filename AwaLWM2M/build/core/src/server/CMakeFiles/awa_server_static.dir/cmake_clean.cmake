FILE(REMOVE_RECURSE
  "liblwm2mserver.pdb"
  "liblwm2mserver.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/awa_server_static.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)

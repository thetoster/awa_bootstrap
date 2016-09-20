FILE(REMOVE_RECURSE
  "liblwm2mserver.pdb"
  "liblwm2mserver.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/awa_serverd_shared.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)

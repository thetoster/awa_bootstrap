FILE(REMOVE_RECURSE
  "liblwm2mbootstrapserver.pdb"
  "liblwm2mbootstrapserver.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/awa_bootstrap_server_shared.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)

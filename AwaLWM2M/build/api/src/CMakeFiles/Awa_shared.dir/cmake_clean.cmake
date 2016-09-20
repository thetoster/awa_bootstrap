FILE(REMOVE_RECURSE
  "libawa.pdb"
  "libawa.so"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/Awa_shared.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)

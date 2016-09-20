FILE(REMOVE_RECURSE
  "libawa.pdb"
  "libawa.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/Awa_static.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)

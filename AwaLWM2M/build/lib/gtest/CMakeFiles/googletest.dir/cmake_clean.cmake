FILE(REMOVE_RECURSE
  "CMakeFiles/googletest"
  "CMakeFiles/googletest-complete"
  "googletest-prefix/src/googletest-stamp/googletest-install"
  "googletest-prefix/src/googletest-stamp/googletest-mkdir"
  "googletest-prefix/src/googletest-stamp/googletest-download"
  "googletest-prefix/src/googletest-stamp/googletest-update"
  "googletest-prefix/src/googletest-stamp/googletest-patch"
  "googletest-prefix/src/googletest-stamp/googletest-configure"
  "googletest-prefix/src/googletest-stamp/googletest-build"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/googletest.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)

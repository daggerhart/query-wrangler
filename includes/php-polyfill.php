<?php

/**
 * https://github.com/Polycademy/upgradephp
 */

/**
 * Inject values from supplemental arrays into $target, according to its keys.
 *
 * @param array  $target
 * @param+ array $supplements
 * @return array
 */
if (!function_exists("array_replace")) {
	function array_replace(/* & (?) */$target/*, $from, $from2, ...*/) {
		$merge = func_get_args();
		array_shift($merge);
		foreach ($merge as $add) {
			foreach ($add as $i=>$v) {
				$target[$i] = $v;
			}
		}
		return $target;
	}
}

/**
 * Descends into sub-arrays when replacing values by key in $target array.
 *
 */
if (!function_exists("array_replace_recursive")) {
	function array_replace_recursive($target/*, $from1, $from2, ...*/) {
		$merge = func_get_args();
		array_shift($merge);
		// loop through all merge arrays
		foreach ($merge as $from) {
			foreach ($from as $i=>$v) {
				// just add (wether array or scalar) if key does not exist yet
				if (!isset($target[$i])) {
					$target[$i] = $v;
				}
				// dive in
				elseif (is_array($v) && is_array($target[$i])) {
					$target[$i] = array_replace_recursive($target[$i], $v);
				}
				// replace
				else {
					$target[$i] = $v;
				}
			}
		}
		return $target;
	}
}

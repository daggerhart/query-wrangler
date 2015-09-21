<?php
/*
 * Where do all these variables come from?
 * They are coming from the arguments sent along with the theme('query_edit', $args) function in query-wrangler.php
 *
 * All keys in the argument array become variables in the template file
 *
 * See the following link for more details on how that works:
 * https://github.com/daggerhart/Query-Wrangler/wiki/Template-Wrangler
 */

?>
<div id="qw-query-admin-options-wrap">
	<!-- left column -->
	<div class="qw-query-admin-column">
		<div id="qw-query-args" class="qw-query-admin-options qw-handler-items">
			<h4>Basic Settings</h4>
			<?php
			foreach ( $basics as $basic ) {
				// no display types set means all querys  || query type is in display types
				if ( ! isset( $basic['query_display_types'] ) || ( in_array( $query_type,
						$basic['query_display_types'] ) )
				) {
					// TODO template this someday
					ob_start();
					?>
					<div id="qw-basic-<?php print $basic['hook_key']; ?>"
					     class="qw-item-form">
						<?php $basic['form_callback']( $basic,
							$options[ $basic['option_type'] ] ); ?>
					</div>
					<?php
					$basic['form'] = ob_get_clean();
					?>
					<div class="qw-handler-item">
						<div
							class="qw-handler-item-title"><?php print $basic['title']; ?></div>
						<div class="qw-handler-item-form">
							<?php print $basic['form']; ?>
						</div>
					</div>
				<?php
				}
			}
			?>
		</div>
		<!-- /qw-query-args -->
		<div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
	</div>
	<!-- /column -->
	<!-- middle column -->
	<div class="qw-query-admin-column">
		<?php
		if ( $query_type == 'override' ) { ?>
			<!-- override settings -->
			<div id="qw-query-overrides" class="qw-query-admin-options">
				<div class="qw-query-add-titles">
            <span class="qw-rearrange-title">
              Rearrange
            </span>
            <span class="qw-add-title" data-handler-type="override"
                  data-limit-per-type=1 data-form-id="qw-display-add-overrides">
              Add
            </span>
				</div>
				<h4>Overrides</h4>

				<div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
				<div id="query-overrides" class="qw-handler-items">
					<?php
					foreach ( $overrides as $override ) { ?>
						<div class="qw-handler-item">
							<div
								class="qw-handler-item-title"><?php print $override['title']; ?></div>
							<div class="qw-handler-item-form">
								<div class="qw-item-form">
									<?php print $override['wrapper_form']; ?>
								</div>
							</div>
						</div>
					<?php
					}
					?>
				</div>
			</div>
			<!-- /override settings -->
		<?php
		}
		?>

		<div id="qw-query-fields" class="qw-query-admin-options">
			<div class="qw-query-add-titles">
        <span class="qw-rearrange-title">
          Rearrange
        </span>
        <span class="qw-add-title" data-handler-type="field"
              data-form-id="qw-display-add-fields">
          Add
        </span>
			</div>

			<h4>Fields</h4>

			<div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
			<div id="query-fields" class="qw-handler-items">
				<?php
				foreach ( $fields as $field ) { ?>
					<div class="qw-handler-item">
						<div
							class="qw-handler-item-title"><?php print $field['title']; ?></div>
						<div class="qw-handler-item-form can-remove">
							<?php print $field['wrapper_form']; ?>
						</div>
					</div>
				<?php
				}
				?>
			</div>
		</div>
		<!-- /fields -->
	</div>
	<!-- /column -->

	<!-- right column -->
	<div class="qw-query-admin-column">
		<!-- sorts -->
		<div id="qw-query-sorts" class="qw-query-admin-options">
			<div class="qw-query-add-titles">
        <span class="qw-rearrange-title">
          Rearrange
        </span>
        <span class="qw-add-title" data-handler-type="sort"
              data-form-id="qw-display-add-sorts">
          Add
        </span>
			</div>
			<h4>Sort Options</h4>

			<div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
			<div id="query-sorts" class="qw-handler-items">
				<?php
				foreach ( $sorts as $sort ) { ?>
					<div class="qw-handler-item">
						<div
							class="qw-handler-item-title"><?php print $sort['title']; ?></div>
						<div class="qw-handler-item-form can-remove">
							<?php print $sort['wrapper_form']; ?>
						</div>
					</div>
				<?php
				}
				?>
			</div>
		</div>

		<!-- filters -->
		<div id="qw-query-filters" class="qw-query-admin-options">
			<div class="qw-query-add-titles">
        <span class="qw-rearrange-title">
          Rearrange
        </span>
        <span class="qw-add-title" data-handler-type="filter"
              data-form-id="qw-display-add-filters">
          Add
        </span>
			</div>

			<h4>Filters</h4>

			<div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

			<div id="query-filters" class="qw-handler-items">
				<?php
				// loop through and display
				foreach ( $filters as $filter_name => $filter ) { ?>
					<div class="qw-handler-item">
						<div
							class="qw-handler-item-title"><?php print $filter['title']; ?></div>
						<div class="qw-handler-item-form can-remove">
							<?php print $filter['wrapper_form']; ?>
						</div>
					</div>
				<?php
				}
				?>
			</div>
		</div>
	</div>
	<div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
</div>

<!-- ------- HIDDEN FORMS --------- -->
<div id="qw-options-forms">
	<!-- all sorts -->
	<div id="qw-display-add-sorts" class="qw-hidden" data-handler-type="sort">
		<p class="description">Select options for sorting the query results.</p>

		<div class="qw-checkboxes">
			<?php
			// loop through sorts
			foreach ( $all_sorts as $hook_key => $sort ) {
				?>
				<label class="qw-sort-checkbox">
					<input type="checkbox"
					       value="<?php print $sort['type']; ?>"/>
					<input class="qw-handler-hook_key"
					       type="hidden"
					       value="<?php print $sort['hook_key']; ?>"/>
					<?php print $sort['title']; ?>
				</label>
				<p class="description"><?php print $sort['description']; ?></p>
			<?php
			}
			?>
		</div>
	</div>

	<!-- all fields -->
	<div id="qw-display-add-fields" data-handler-type="field">
		<p class="description">Select Fields to add to this query's output.</p>

		<div class="qw-checkboxes">
			<?php
			// loop through fields
			foreach ( $all_fields as $hook_key => $field ) {
				?>
				<label class="qw-field-checkbox">
					<input type="checkbox"
					       value="<?php print $field['type']; ?>"/>
					<input class="qw-handler-hook_key"
					       type="hidden"
					       value="<?php print $field['hook_key']; ?>"/>
					<?php print $field['title']; ?>
				</label>
				<p class="description"><?php print $field['description']; ?></p>
			<?php
			}
			?>
		</div>
	</div>

	<!-- all filters -->
	<div id="qw-display-add-filters" data-handler-type="filter">
		<p class="description">Select filters to affect the query's results.</p>

		<div class="qw-checkboxes">
			<?php
			// loop through filters
			foreach ( $all_filters as $hook_key => $filter ) {
				// for now, this is how I'll prevent certain filters on overrides
				if ( isset( $filter['query_display_types'] ) && is_array( $filter['query_display_types'] ) && in_array( $query_type,
						$filter['query_display_types'] )
				) { ?>
					<label class="qw-filter-checkbox">
						<input type="checkbox"
						       value="<?php print $filter['type']; ?>"/>
						<input class="qw-handler-hook_key"
						       type="hidden"
						       value="<?php print $filter['hook_key']; ?>"/>
						<?php print $filter['title']; ?>
					</label>
					<p class="description"><?php print $filter['description']; ?></p>
				<?php
				}
			}
			?>
		</div>
	</div>

	<!-- all overrides -->
	<div id="qw-display-add-overrides" data-handler-type="override">
		<p class="description">
			Select overrides to add to this query.
			Limit 1 per type.</p>

		<div class="qw-checkboxes">
			<?php
			// loop through overrides
			foreach ( $all_overrides as $hook_key => $override ) {
				?>
				<label class="qw-override-checkbox">
					<input type="checkbox"
					       value="<?php print $override['type']; ?>"/>
					<input class="qw-handler-hook_key"
					       type="hidden"
					       value="<?php print $override['hook_key']; ?>"/>
					<?php print $override['title']; ?>
				</label>
				<p class="description"><?php print $override['description']; ?></p>
			<?php
			}
			?>
		</div>
	</div>
</div><!-- options forms -->

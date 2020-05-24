<?php

namespace QueryWrangler\Admin\MetaBox;

use Kinglet\Admin\MetaBoxBase;
use Kinglet\Form\FormFactory;
use Kinglet\Registry\RegistryRepositoryInterface;
use QueryWrangler\QueryPostEntity;

class QueryDetails extends MetaBoxBase {

	/**
	 * @var RegistryRepositoryInterface
	 */
	protected $settings;

	/**
	 * @var QueryPostEntity
	 */
	protected $query;

    /**
     * @var FormFactory
     */
    protected $formFactory;

    /**
     * Preview constructor.
     *
     * @param string|string[] $post_types
     * @param RegistryRepositoryInterface $settings
     * @param FormFactory $form_factory
     */
    public function __construct( $post_types, RegistryRepositoryInterface $settings, FormFactory $form_factory ) {
        parent::__construct( $post_types );
        $this->settings = $settings;
        $this->formFactory = $form_factory;
    }

	/**
	 * {@inheritdoc}
	 */
	public function id() {
		return 'query-details';
	}

	/**
	 * {@inheritdoc}
	 */
	public function title() {
		return __( 'Details', 'query-wrangler' );
	}

	/**
	 * {@inheritdoc}
	 */
	public function render( $post ) {
        $is_new = empty( $post->post_title );
        $this->d(qw_all_post_statuses());
		$this->d(qw_get_all_widgets());

        $this->query = new QueryPostEntity( $post );
        $this->d($this->query);
        ?>
		<code><?php echo $this->query->slug(); ?></code>
		<?php
	}

}

<?php
    /* all live-data in:
    sites/all/modules/a_wusel_migration/data_sources/
    */
    /* based on: drupal.org/node/1269066#comment-4988994 */
    /* and:      drupal.org/node/1190958#comment-4616032 */
class ImportUsersMigration extends Migration {
  public function __construct() {
    // Always call the parent constructor first for basic setup
    //parent::__construct(MigrateGroup::getInstance('UserMigrate'));
    parent::__construct();

    $this->description = t('Import an CSV-file (only "Account"-fields)');
    $columns = array(
      // "Source": ('Fieldname', 'Description')
      0 => array('id', t('Member-ID (must be unique)')),
      1 => array('first_name', t('First Name')),
      2 => array('last_name', t('Last Name')),
      3 => array('email', t('Email (Account)')),
      4 => array('name', t('Full Name')),
      5 => array('created', t('Created'))
    );
    // TIP: delete ", array('header_rows' => 1)" in the next line, when the CSV-file has NO header-line
    $this->source = new MigrateSourceCSV(DRUPAL_ROOT . '/' . drupal_get_path('module', 'importusers') . '/data_sources/drupaluser_import.csv', $columns, array('header_rows' => 1));
    $this->destination = new MigrateDestinationUser();
    $this->map = new MigrateSQLMap($this->machineName,
        array('id' => array( // this field is used to connect user und profile2
                'type' => 'int',
                'unsigned' => TRUE,
                'not null' => TRUE,
                'description' => t('User\'s ID') // description never used
              )
             ),
        MigrateDestinationUser::getKeySchema()
    );

    // Mapped fields
    //
    $this->addSimpleMappings(array('name', 'created'));
    $this->addFieldMapping('mail', 'email')
      ->defaultValue('replace@me.com');
    $this->addFieldMapping('field_firstname', 'first_name')
      ->defaultValue('');
    $this->addFieldMapping('field_lastname', 'last_name')
      ->defaultValue('');
    $this->addFieldMapping('password')
      ->defaultValue('asdf');
    $this->addFieldMapping('is_new')
      ->defaultValue(TRUE);
    $this->addFieldMapping('roles')
      ->defaultValue(2); // 2 = "authenticated user"
    $this->addFieldMapping('theme')
      ->defaultValue('');
    $this->addFieldMapping('signature')
      ->defaultValue('');
    $this->addFieldMapping('signature_format')
      ->defaultValue('filtered_html');
    $this->addFieldMapping('access')
      ->defaultValue(0);
    $this->addFieldMapping('login')
      ->defaultValue(0);
    $this->addFieldMapping('status')
      ->defaultValue(1);
    $this->addFieldMapping('timezone')
      ->defaultValue(NULL);
    $this->addFieldMapping('language')
      ->defaultValue(t('en')); // e.g.: 'en', 'fr', 'de', ...
    $this->addFieldMapping('picture')
      ->defaultValue(0);
    $this->addFieldMapping('init', 'mail');
   
    // Other handlers
    $this->addFieldMapping('path')
      ->defaultValue(NULL);
    $this->addFieldMapping('pathauto')
      ->defaultValue(''); // Perform aliasing (set to 0 to prevent alias generation during migration)
  }
}

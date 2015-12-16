<?

/* Database names defined as constants */

define('DB_LAST_UPDATE','lastUpd');

define('DB_SECTION_TABLE','xc_sections');
define('DB_SECTION_ID','xcs_id');
define('DB_SECTION_NAME','xcs_name');
define('DB_SECTION_TITLE','xcs_title');
define('DB_SECTION_SHORT_DESCRIPTION','xcs_short_description');
define('DB_SECTION_DESCRIPTION','xcs_description');
define('DB_SECTION_PREVIOUS','xcs_previous');
define('DB_SECTION_TIME','xcs_time');
define('DB_SECTION_PRIORITY','xcs_priority');
define('DB_SECTION_SUBPRIORITY','xcs_subpriority');
define('DB_SECTION_INVISIBLE','xcs_invisible');
define('DB_SECTION_LOGO','xcs_logo');
define('DB_SECTION_URL','xcs_url');
define('DB_SECTION_OPTIONS','xcs_options');

/* section options */

define('S_OPTION_INVISIBLE',1);

define('DB_ARTICLE_TABLE','xc_articles');
define('DB_ARTICLE_ID','xca_id');
define('DB_ARTICLE_NAME','xca_name');
define('DB_ARTICLE_AUTHOR','xca_author');
define('DB_ARTICLE_PEREX','xca_perex');
define('DB_ARTICLE_TEXT','xca_text');
define('DB_ARTICLE_TIME','xca_time');
define('DB_ARTICLE_UTIME','utime');
define('DB_ARTICLE_READERS','xca_readers');
define('DB_ARTICLE_SECTION','xca_section');
define('DB_ARTICLE_PRIORITY','xca_priority');

define('DB_LINX_TABLE','xc_linx');
define('DB_LINX_ID','xcl_id');
define('DB_LINX_URL','xcl_url');
define('DB_LINX_NAME','xcl_name');
define('DB_LINX_DESCRIPTION','xcl_description');
define('DB_LINX_SECTION','xcl_section');

define('DB_TEXT_TABLE','xc_text');
define('DB_TEXT_ID','xct_id');
define('DB_TEXT_HEADER','xct_header');
define('DB_TEXT_TEXT','xct_text');
define('DB_TEXT_SECTION','xct_section');
define('DB_TEXT_TYPE','xct_type');
define('DB_TEXT_PRIORITY','xct_priority');

define('DB_XOTD_TABLE','xc_xoftheday');
define('DB_XOTD_ID','xcx_id');
define('DB_XOTD_NAME','xcx_name');
define('DB_XOTDD_TABLE','xcx_data');
define('DB_XOTDD_XOTD_ID','xcxd_xcx_id');
define('DB_XOTDD_CONTENT','xcxd_content');
define('DB_XOTD_SECTIONS_TABLE','xcx_sections');
define('DB_XOTD_SECTIONS_SECTION_ID','xcxs_xcs_id');
define('DB_XOTD_SECTIONS_XOTD_ID','xcxs_xcx_id');
define('DB_XOTD_SECTIONS_PRIORITY','xcxs_priority');

define('DB_XTUIT_TABLE','xc_tuit');
define('DB_XTUIT_SECTION','xct_xcs_id');
define('DB_XTUIT_CONTENTS','xct_content');

define('DB_EXERCISES_TABLE','exercises');
define('DB_EXERCISE_ID','ex_id');
define('DB_EXERCISE_NAME','ex_name');
define('DB_EXERCISE_DESCRIPTION','ex_description');
define('DB_EXERCISE_CREATOR','ex_creator');
define('DB_EXERCISE_TASK','ex_task');
define('DB_EXERCISE_STORY','ex_story');
define('DB_EXERCISE_DIFFICULTY','ex_difficulty');
define('DB_EXERCISE_OPTIONS','ex_options');

define('DB_UNIT_TABLE','sets');
define('DB_UNIT_ID','set_id');
define('DB_UNIT_TYPE','set_type');
define('DB_UNIT_NAME','set_name');
define('DB_UNIT_CREATOR','set_creator');
define('DB_UNIT_DIFFICULTY','set_difficulty');

define('DB_EXERCISE_TYPE_TABLE','exercise_types');
define('DB_EXERCISE_TYPE_ID','et_id');
define('DB_EXERCISE_TYPE_NAME','et_name');
define('DB_EXERCISE_TYPE_SHORT','et_short');

define('DB_EXERCISE_CATEGORIES_TABLE','exercise_categories');
define('DB_EXERCISE_CATEGORIES_EX_ID','ec_ex_id');
define('DB_EXERCISE_CATEGORIES_ET_ID','ec_et_id');

/* Other useful constants */

define('MAX_DAYS_FOR_NEW',7);
define('NEW_INFO_TEXT','<img src="images/newinfo.gif" border=0 alt="New info added">');
define('DEFAULT_PRIORITY',100);
define('TEXTBOX',0);
define('TEXTBOX_SMALL',1);

?>

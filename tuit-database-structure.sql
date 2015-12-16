-- phpMyAdmin SQL Dump
-- version 2.8.1
-- http://www.phpmyadmin.net
-- 
-- Host: uvdb1.globe.cz
-- Generation Time: Sep 30, 2006 at 10:10 PM
-- Server version: 5.0.22
-- PHP Version: 4.3.10-10ubuntu4.7
-- 
-- Database: `bohemicaco`
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `acct_requests`
-- 

CREATE TABLE `acct_requests` (
  `req_id` int(11) NOT NULL auto_increment,
  `req_login` varchar(12) NOT NULL default '',
  `req_passwd` varchar(16) NOT NULL default '',
  `req_name` varchar(30) NOT NULL default '',
  `req_email` varchar(30) NOT NULL default '',
  `req_time` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  PRIMARY KEY  (`req_id`),
  FULLTEXT KEY `req_login` (`req_login`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1686 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `an_choose`
-- 

CREATE TABLE `an_choose` (
  `ac_id` int(11) NOT NULL auto_increment,
  `ac_an_id` int(11) NOT NULL default '0',
  `ac_answer` tinyint(4) NOT NULL default '0',
  `ac_correct` tinyint(4) default NULL,
  PRIMARY KEY  (`ac_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `an_complete`
-- 

CREATE TABLE `an_complete` (
  `aco_id` int(11) NOT NULL auto_increment,
  `aco_an_id` int(11) NOT NULL default '0',
  `aco_answer` text,
  `aco_type` tinyint(4) default NULL,
  `aco_correct` text,
  `aco_points` text,
  PRIMARY KEY  (`aco_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=63 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `an_crossword`
-- 

CREATE TABLE `an_crossword` (
  `acr_id` int(11) NOT NULL auto_increment,
  `acr_an_id` int(11) NOT NULL default '0',
  `acr_answer` text,
  `acr_correct` text,
  `acr_translation_correct` tinyint(4) default NULL,
  PRIMARY KEY  (`acr_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `an_match`
-- 

CREATE TABLE `an_match` (
  `am_id` int(11) NOT NULL auto_increment,
  `am_an_id` int(11) NOT NULL default '0',
  `am_answer` tinyint(4) NOT NULL default '0',
  `am_correct` tinyint(4) default NULL,
  PRIMARY KEY  (`am_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `an_qanda`
-- 

CREATE TABLE `an_qanda` (
  `aq_id` int(11) NOT NULL auto_increment,
  `aq_an_id` int(11) NOT NULL default '0',
  `aq_answer` varchar(160) default NULL,
  `aq_correct` tinyint(4) default NULL,
  PRIMARY KEY  (`aq_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `answers`
-- 

CREATE TABLE `answers` (
  `an_id` int(11) NOT NULL auto_increment,
  `an_ex_id` int(11) NOT NULL default '0',
  `an_who_id` int(11) NOT NULL default '0',
  `an_as_id` int(11) NOT NULL default '0',
  `an_corr_by` int(11) NOT NULL default '0',
  `an_corr_when` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `an_points` float NOT NULL default '0',
  `an_comment` text,
  PRIMARY KEY  (`an_id`),
  UNIQUE KEY `an_ex_id` (`an_ex_id`,`an_who_id`,`an_as_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=52 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `article_authors`
-- 

CREATE TABLE `article_authors` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(100) NOT NULL default '',
  `email` varchar(100) NOT NULL default '',
  `byline` text NOT NULL,
  `pic_url` varchar(100) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=2 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `article_cats`
-- 

CREATE TABLE `article_cats` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(100) NOT NULL default '',
  `description` text NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=3 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `article_lookup`
-- 

CREATE TABLE `article_lookup` (
  `JID` int(11) NOT NULL default '0',
  `CID` int(11) NOT NULL default '0',
  PRIMARY KEY  (`JID`,`CID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2;

-- --------------------------------------------------------

-- 
-- Table structure for table `articles`
-- 

CREATE TABLE `articles` (
  `id` tinyint(4) NOT NULL auto_increment,
  `description` text NOT NULL,
  `title` varchar(200) NOT NULL default '',
  `content` text NOT NULL,
  `aid` int(11) NOT NULL default '0',
  `datecreated` date NOT NULL default '0000-00-00',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=2 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `as_tasks`
-- 

CREATE TABLE `as_tasks` (
  `at_id` int(11) NOT NULL auto_increment,
  `at_task_id` int(11) NOT NULL default '0',
  `at_as_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`at_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=39 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `as_templates`
-- 

CREATE TABLE `as_templates` (
  `atm_id` int(11) NOT NULL auto_increment,
  `atm_name` varchar(80) default NULL,
  `atm_query` text,
  PRIMARY KEY  (`atm_id`),
  KEY `atm_name` (`atm_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `as_who`
-- 

CREATE TABLE `as_who` (
  `aw_id` int(11) NOT NULL auto_increment,
  `aw_who_id` int(11) NOT NULL default '0',
  `aw_as_id` int(11) NOT NULL default '0',
  `aw_complete` tinyint(4) NOT NULL default '0',
  `aw_active` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`aw_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=32 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `as_who_task_complete`
-- 

CREATE TABLE `as_who_task_complete` (
  `awtc_who` int(11) NOT NULL default '0',
  `awtc_task` int(11) NOT NULL default '0',
  `awtc_complete` tinyint(4) default NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin2;

-- --------------------------------------------------------

-- 
-- Table structure for table `assignments`
-- 

CREATE TABLE `assignments` (
  `as_id` int(11) NOT NULL auto_increment,
  `as_description` varchar(200) default NULL,
  `as_task_type` tinyint(4) NOT NULL default '0',
  `as_on` tinyint(4) NOT NULL default '0',
  `as_on_after` int(11) default NULL,
  `as_modifier` tinyint(4) default NULL,
  `as_limit` varchar(10) default NULL,
  `as_who_type` tinyint(4) default NULL,
  `as_by` int(11) NOT NULL default '0',
  `as_when` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `as_complete` tinyint(4) default NULL,
  `as_active` tinyint(4) NOT NULL default '1',
  `as_comment` text,
  PRIMARY KEY  (`as_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=32 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `configuration`
-- 

CREATE TABLE `configuration` (
  `conf_key` varchar(20) NOT NULL default '',
  `conf_value` text,
  `conf_description` varchar(100) default NULL,
  PRIMARY KEY  (`conf_key`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2;

-- --------------------------------------------------------

-- 
-- Table structure for table `dict1_2`
-- 

CREATE TABLE `dict1_2` (
  `d1_id` int(11) NOT NULL auto_increment,
  `d1_word1` varchar(100) NOT NULL default '',
  `d1_word2` varchar(100) NOT NULL default '',
  `d1_note1` varchar(20) default NULL,
  `d1_note2` varchar(20) default NULL,
  `d1_note3` varchar(20) default NULL,
  `d1_lesson` varchar(20) default NULL,
  `d1_sound` varchar(20) default NULL,
  `d1_usage` varchar(200) default NULL,
  PRIMARY KEY  (`d1_id`),
  KEY `w1_w2` (`d1_word1`,`d1_word2`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=577 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `dicts`
-- 

CREATE TABLE `dicts` (
  `d_id` int(11) NOT NULL auto_increment,
  `d_name` varchar(50) NOT NULL default '',
  `d_desc` text,
  PRIMARY KEY  (`d_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=4 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `difficulty`
-- 

CREATE TABLE `difficulty` (
  `dif_id` tinyint(4) NOT NULL auto_increment,
  `dif_name` varchar(20) default NULL,
  PRIMARY KEY  (`dif_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=4 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `ex_choose`
-- 

CREATE TABLE `ex_choose` (
  `ch_id` int(11) NOT NULL auto_increment,
  `ch_ex_id` int(11) NOT NULL default '0',
  `ch_question` varchar(160) default NULL,
  `ch_choices` text,
  `ch_correct` tinyint(4) NOT NULL default '0',
  PRIMARY KEY  (`ch_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=19 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `ex_match`
-- 

CREATE TABLE `ex_match` (
  `ma_id` int(11) NOT NULL auto_increment,
  `ma_ex_id` int(11) NOT NULL default '0',
  `ma_first` varchar(160) default NULL,
  `ma_second` varchar(160) default NULL,
  PRIMARY KEY  (`ma_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=6 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `ex_qanda`
-- 

CREATE TABLE `ex_qanda` (
  `qa_id` int(11) NOT NULL auto_increment,
  `qa_ex_id` int(11) NOT NULL default '0',
  `qa_question` varchar(160) default NULL,
  `qa_answer` varchar(160) default NULL,
  PRIMARY KEY  (`qa_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=277 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `exercise_categories`
-- 

CREATE TABLE `exercise_categories` (
  `ec_ex_id` int(11) default NULL,
  `ec_et_id` int(11) default NULL,
  UNIQUE KEY `ec_ex_id` (`ec_ex_id`,`ec_et_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2;

-- --------------------------------------------------------

-- 
-- Table structure for table `exercise_types`
-- 

CREATE TABLE `exercise_types` (
  `et_id` int(11) NOT NULL auto_increment,
  `et_short` varchar(12) default NULL,
  `et_name` varchar(80) NOT NULL default '',
  PRIMARY KEY  (`et_id`),
  UNIQUE KEY `et_name` (`et_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=180 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `exercises`
-- 

CREATE TABLE `exercises` (
  `ex_id` int(11) NOT NULL auto_increment,
  `ex_type` tinyint(4) NOT NULL default '0',
  `ex_name` varchar(80) default NULL,
  `ex_description` varchar(255) default NULL,
  `ex_creator` int(11) NOT NULL default '0',
  `ex_task` varchar(80) default NULL,
  `ex_story` text,
  `ex_difficulty` tinyint(4) default NULL,
  `ex_options` varchar(255) default NULL,
  PRIMARY KEY  (`ex_id`),
  KEY `ex_name` (`ex_name`),
  KEY `type_creator` (`ex_type`,`ex_creator`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=2357 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `exercises_sets`
-- 

CREATE TABLE `exercises_sets` (
  `se_id` int(11) NOT NULL auto_increment,
  `se_ex_id` int(11) NOT NULL default '0',
  `se_set_id` int(11) NOT NULL default '0',
  `se_ss_id` int(11) default NULL,
  `se_seq` int(11) default NULL,
  PRIMARY KEY  (`se_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=545 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `formatting`
-- 

CREATE TABLE `formatting` (
  `f_id` int(11) NOT NULL auto_increment,
  `f_name` varchar(100) default NULL,
  PRIMARY KEY  (`f_id`),
  UNIQUE KEY `f_name` (`f_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `genus`
-- 

CREATE TABLE `genus` (
  `g_id` int(11) NOT NULL auto_increment,
  `g_name` varchar(255) NOT NULL default '',
  PRIMARY KEY  (`g_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `messages`
-- 

CREATE TABLE `messages` (
  `id` int(11) NOT NULL auto_increment,
  `board_id` int(11) NOT NULL default '0',
  `author` varchar(50) default NULL,
  `email` varchar(50) default NULL,
  `subject` varchar(160) default NULL,
  `text` text,
  `sent` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `following` int(11) NOT NULL default '0',
  `send_note` tinyint(4) default '0',
  PRIMARY KEY  (`id`),
  KEY `board_id` (`board_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=1906 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `notes`
-- 

CREATE TABLE `notes` (
  `n_id` int(11) NOT NULL auto_increment,
  `n_who_id` int(11) NOT NULL default '0',
  `n_text` text,
  `n_description` varchar(100) default NULL,
  `n_my_URL` varchar(255) default NULL,
  PRIMARY KEY  (`n_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=5 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `parameters`
-- 

CREATE TABLE `parameters` (
  `id` int(11) NOT NULL auto_increment,
  `name` varchar(80) NOT NULL default '',
  `description` varchar(255) default NULL,
  `count` int(11) NOT NULL default '0',
  `anonymous` tinyint(4) default NULL,
  `modified` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `admin_notify` tinyint(4) default NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=23 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `permissions`
-- 

CREATE TABLE `permissions` (
  `perm_id` int(11) NOT NULL default '0',
  `perm_name` varchar(30) default NULL,
  `perm_str` varchar(50) default NULL,
  PRIMARY KEY  (`perm_id`),
  UNIQUE KEY `perm_name` (`perm_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2;

-- --------------------------------------------------------

-- 
-- Table structure for table `sd_d1`
-- 

CREATE TABLE `sd_d1` (
  `sdd_sd_id` int(11) NOT NULL default '0',
  `sdd_d1_id` int(11) NOT NULL default '0',
  PRIMARY KEY  (`sdd_sd_id`,`sdd_d1_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2;

-- --------------------------------------------------------

-- 
-- Table structure for table `set_sections`
-- 

CREATE TABLE `set_sections` (
  `ss_id` int(11) NOT NULL auto_increment,
  `ss_set_id` int(11) NOT NULL default '0',
  `ss_name` varchar(50) default NULL,
  `ss_seq` int(11) NOT NULL default '0',
  PRIMARY KEY  (`ss_id`),
  UNIQUE KEY `id_name` (`ss_set_id`,`ss_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=186 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `set_types`
-- 

CREATE TABLE `set_types` (
  `st_id` tinyint(4) NOT NULL auto_increment,
  `st_name` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`st_id`),
  UNIQUE KEY `st_name` (`st_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=15 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `sets`
-- 

CREATE TABLE `sets` (
  `set_id` int(11) NOT NULL auto_increment,
  `set_type` tinyint(4) NOT NULL default '0',
  `set_name` varchar(80) default NULL,
  `set_creator` int(11) NOT NULL default '0',
  `set_difficulty` tinyint(4) default NULL,
  PRIMARY KEY  (`set_id`),
  KEY `set_name` (`set_name`),
  KEY `type_creator` (`set_type`,`set_creator`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=38 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `ss_templates`
-- 

CREATE TABLE `ss_templates` (
  `sst_id` int(11) NOT NULL auto_increment,
  `sst_name` varchar(80) default NULL,
  `sst_insert` text,
  PRIMARY KEY  (`sst_id`),
  KEY `sst_name` (`sst_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=8 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `st_dict`
-- 

CREATE TABLE `st_dict` (
  `sd_id` int(11) NOT NULL auto_increment,
  `sd_student_id` int(11) NOT NULL default '0',
  `sd_name` varchar(100) NOT NULL default '',
  PRIMARY KEY  (`sd_id`),
  KEY `sd_name` (`sd_name`),
  KEY `sd_student_id` (`sd_student_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=5 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `users`
-- 

CREATE TABLE `users` (
  `id` int(11) NOT NULL auto_increment,
  `login` varchar(12) NOT NULL default '',
  `passwd` varchar(16) NOT NULL default '',
  `name` varchar(30) NOT NULL default '',
  `email` varchar(30) NOT NULL default '',
  `send_note` tinyint(4) default NULL,
  `default_teacher` int(11) NOT NULL default '0',
  `admin` tinyint(4) default '0',
  `cookie` varchar(32) default NULL,
  `ip` varchar(15) default NULL,
  `last_change` timestamp NOT NULL default CURRENT_TIMESTAMP on update CURRENT_TIMESTAMP,
  `expiry` date default NULL,
  `logins` int(11) NOT NULL default '0',
  `last_login` timestamp NOT NULL default '0000-00-00 00:00:00',
  PRIMARY KEY  (`id`),
  UNIQUE KEY `login` (`login`),
  KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=60 ;

-- --------------------------------------------------------

-- 
-- Table structure for table `vocab`
-- 

CREATE TABLE `vocab` (
  `v_id` int(11) NOT NULL auto_increment,
  `v_d_id` int(11) NOT NULL default '0',
  `v_czech` varchar(100) default NULL,
  `v_english` varchar(100) default NULL,
  `v_latin` varchar(100) default NULL,
  `v_german` varchar(100) default NULL,
  `v_slovak` varchar(100) default NULL,
  `v_note` varchar(255) default NULL,
  `v_g_id` int(11) default NULL,
  PRIMARY KEY  (`v_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin2 AUTO_INCREMENT=5577 ;

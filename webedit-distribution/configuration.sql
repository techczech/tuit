# MySQL dump 8.12
#
# Host: localhost    Database: bohemicaco
#--------------------------------------------------------
# Server version	3.23.33

#
# Table structure for table 'configuration'
#

CREATE TABLE configuration (
  conf_key varchar(20) NOT NULL default '',
  conf_value text,
  conf_description varchar(100) default NULL,
  PRIMARY KEY (conf_key)
) TYPE=MyISAM;

#
# Dumping data for table 'configuration'
#

INSERT INTO configuration VALUES ('server_name','Bohemica.com','Server name shown in titles.');
INSERT INTO configuration VALUES ('topmenu','| <a href=\"http://www.bohemica.com\">Bohemica.com</a> |','HTML code representing top menu');
INSERT INTO configuration VALUES ('admin_email','micval@temnet.org','Administrator\'s email');
INSERT INTO configuration VALUES ('server_addr','http://www.bohemica.com','Base server URL');
INSERT INTO configuration VALUES ('emails_from','czechonline@bohemica.com','Email address used in From: header of sent emails');
INSERT INTO configuration VALUES ('default_logo','images/bohemica_top.jpg','Path to image of default site logo');


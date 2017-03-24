## Project
MCL (Mainlab Chado Loader) is a module that enables users to upload their biological data to chado database schema. Users are required to transfer their biological data into various types of data template files. MCL, then, uploads these data template files into a chado schema. MCL requires each type of data template to have a corresponding class file. MCL predefines the class files for major data template types such as marker, QTL, germplasm, map, project. The flexibility of Chado schema often allows, however, the same type of biological data to be stored in various ways.. When their data are modeled and stored differently in Chado, users can modify these predefined class files. Otherwise users can inherit the existing template class. MCL also allow for users to define a new class file for a new data template. The details of adding a new template are described in "Customization" section.

MCL provides a "Data Template" page that shows the list of all the types of data template that MCL currently supports. A user can view and download each data template file from the list. MCL also provides "Upload Data" page. See more details in "How to upload data" section below. These two pages are linked from the main page of MCL (http://your.site/mcl).

The Mainlab Chado Loader is created by Main Bioinformatics Lab (Main Lab) at Washington State University. Information about the Main Lab can be found at: https://www.bioinfo.wsu.edu

## Requirement
 - Drupal 7.x

## Version
1.0.0

##Download
The MainLab Chado Loader module can be download from GitHub:

https://github.com/tripal/mainlab_chado_loader

## Installation
After downloading the module, extract it into your site's module directory 
(e.g. sites/all/modules) then follow the instructions below:

  1. Enable the module by using the Drupal administrative interface: Go to: Modules, check 'mcl' (under the MCL) and save or by using the 'drush' command:
     ```
     drush pm-enable mcl
     ```
     This will create all MCL related tables in public schema and populate the tables with default values. It will also create directories for MCL in Drupal public file directory.

## Administration
 - Adding/Deleting template types
 
   A new template type can be added or an existing type can be deleted. The details of adding a new template type are described in "Customization" section.

 - Adding/Deleting templates
 
   A new template can be added or an existing template can be deleted. The details of adding a new template are described in "Customization" section.

 - Adding/Deleting users
 
   Site visitors who can upload data to chado schema can be restricted by adding or deleting uploading privilege to a Drupal account holder.

 - Setting MCL global variables.
 
    MCL has two types of global variables.

     1. Site specific variables.

        MCL requires a set of the site specific variables for uploading data. An admin page is available for users to assign or change the values for these variables.

    2. Default / Dummy variables.

       Some columns of Chado tables have NOT NULL constraint but there can be no valid data to enter in some databases. In this situation, users can assign default values for these columns using an MCL admin page.

## Customization
1. Add a new template type in the admin page.

    a. Assign a new name.
    
    It must be unique.

    b. Assign a rank.

    The rank of the template type determines the order of templates to be uploaded. Some of data in a template may depend on data on other templates. So data dependency must be taken into consideration when you assign a rank to the new template type.


2. Add a new template.

  Users add a new template if they do not want to modify MCL pre-defined template classes.
  
    a. Template Class file.
 
      i. Create a new class file for a new template.
      Users can inherit the pre-defined template class or simply inherit base class of MCL template class (MCL_TEMPATE). The file name of the new class must be the same as the name of the new template name. A class name must be all capitalized and a file name must be lowercase.
     ```
      (e.g.) class name MCL_TEMPALTE_STOCK_GDR 
      The file name should be 'mcl_template_stock_gdr.inc'
      ```
      ii. Place the newly created class file under the template directory.
      The template directory is 'mcl/include/class/template/module/'.
  
    b. Go to the admin page.
    
      i. Select a template type.

      ii. Assign a new template name.
      A name of a new template must be unique.
      
## How to upload data.
The primary function of this module is to upload biological data to Chado schema. This section covers how to use this module to upload data. There are several steps to complete data uploading.

  1. Map data to MCL data templates.
  First, the biological data must be transferred to the current existing data templates. If data do not fit to our pre-defined data templates, modify the existing ones or create a new data template. See the "Customization" section above.


  2. Upload data templates.
  MCL provides a web form to upload data template files in "Upload Data" page. The hyperlink to this page can be found in the MCL home page. In this page, users can upload their data template files and start running the data uploading job.
    
MCL data-uploading phases :

    Phase 1: checks syntax errors.
    It checks syntax errors such as missing columns, missing data in required columns, and miss-spelled column name.

    Phase 2: checks data errors.
    The data errors are the errors on the data in database. Chado tables have many foreign key  relationships and when the data is not found in the parent tables, the loader module throws an error. For instance, genus and species are listed in stock data template. MCL checks if organism_id for these genus and species exists in the organism table. If not found, it throws an error and it adds an appropriate error message to the error log file.

    Phase 3: uploads data templates.
    After checking errors, it finally starts uploading data in the template to the Chado database.

During the uploading phase, MCL creates several different types of log files and stores in the job directory. MCL seizes the process if it finds an error and ask the user to fix the data. The user goes through all the log files to find the errors and fix them. After fixing the error, the user can come back to the "Upload Job" page to re-upload the fixed data file. Then, the user can re-run the job.

  3. Check log files.
  After the completion of the uploading data job, the user needs to view the log file for newly added data to make sure that data have been added into the database.

## Problems/Suggestions
Mainlab Chado Loader is still under active development. For questions or bug report, please contact the developers at the Main Bioinformatics Lab by emailing to: dev@bioinfo.wsu.edu

X->ZZ->2l2nu Analysis Package
===============================

  Analysis package heavy resonnance search using X->ZZ->2l2nu final states.
 
  For 76X analysis.

Instructions for package development.
---------------------------------

1. Fork the following two repositories into your own github directory if you didn't do this before:

   Note: do this on the webpage, just click the links below and find the "Fork" button on the webpage.

   The Heppy Framework:
   https://github.com/mmhy/cmg-cmssw.git

   The analysis package:
   https://github.com/mmhy/cmgtools-lite.git

2. Setup Environment

  ```
  release=CMSSW_7_6_3_patch2
  tag=""
  export SCRAM_ARCH=slc6_amd64_gcc493
  alias cmsenv='eval `scramv1 runtime -sh`'
  alias cmsrel='scramv1 project CMSSW'

  scram project -n ${release}${tag} $release
  cd ${release}${tag}/src
  cmsenv
  ```

3. Create empty repository (with the cmssw trick to keep the repository small)

  ```
  git cms-init
  ```

4. Add MMHY repository which contains the CMGTools/XZZ2l2nu package, and fetch it

  ```
  git remote add mmhy https://github.com/mmhy/cmg-cmssw.git
  git fetch mmhy
  ```

5. Configure the sparse checkout (to only checkout needed packages), and check out only the Heppy framework

  ```
  curl -O https://raw.githubusercontent.com/mmhy/cmgtools-lite/xzz2l2nu_76x/XZZ2l2nu/tools/sparse-checkout
  mv sparse-checkout .git/info/sparse-checkout
  git checkout -b xzz2l2nu_heppy_76X mmhy/xzz2l2nu_heppy_76X
  ```

  [the rest in this section is not need for everyday life.]

  Optionally, you can store the Heppy branch into your own repository if you need to do some developement on the Heppy framework directly:

  ```
  git remote add origin https://github.com/<your own github name>/cmg-cmssw.git
  git push origin xzz2l2nu_heppy_76X 
  ```
  Don't for get to replace "<your own github name>" with your own github user name.

  Create a developement branch:

  ```
  git checkout -b xzz2l2nu_heppy_76X_dev
  ```

  Do modifications, commit changes, and push to your own repository:

  ```
  git commit -m 'message of the change' -a
  git push origin xzz2l2nu_heppy_76X_dev
  ```

  To update the mmhy repository with your own Heppy framework developement, similar way is required as described in section 12 below. 

  **If ever possible, please don't change the Heppy framework.**

6. Checkout the CMGTools/XZZ2l2nu package, currently the main branch is xzz2l2nu_76x.

  From 76x, the CMGTools is made as a standalone package. So we need to add repository separately:

  ```
  git clone -o mmhy https://github.com/mmhy/cmgtools-lite.git -b xzz2l2nu_76x CMGTools
  ```

7. Compile the package together with Heppy Framework:
   Note, do this in CMSSWxxx/src instead of your CMSSWxxx/src/CMSTools/ directory.

  ```
  scram b -j 3
  ```


8. Add your repository, and store the main branch in your repository:

  Note, do this in your CMSSWxxx/src/CMSTools/ directory, not the CMSSWxxx/src.
  And, don't for get to replace "<your own github name>" with your own github user name.
  ```
  cd CMGTools
  git remote add origin https://github.com/<your own github name>/cmgtools-lite.git
  git push origin xzz2l2nu_76x
  ```


9. Make a copy of branch xzz2l2nu_76x for your own developement, you can choose a branch name as you want, such as xzz2l2nu_76x_dev

  Note, do this in your CMSSWxxx/src/CMSTools/ directory, not the CMSSWxxx/src.
  ```
  git checkout -b xzz2l2nu_76x_dev
  ```

10. Please frequently commit your changes and push your development branch to your own repository

  Note, do this in your CMSSWxxx/src/CMSTools/ directory, not the CMSSWxxx/src.
  ```
  git commit -m 'describe your change here.' -a
  git push origin xzz2l2nu_76x_dev
  ```


11. Once your developement is done, you can update the central branch xzz2l2nu_76x with a Pull Request. Steps below:

  Note, do all following in your CMSSWxxx/src/CMSTools/ directory, not the CMSSWxxx/src.

  * Update the branch xzz2l2nu_76x in your local repository with others developements on mmhy
    ```
    git checkout xzz2l2nu_76x
    git fetch mmhy 
    ```

  * Rebased your development branch to the head of xzz2l2nu_76x, and update it in your own repository
    ```
    git checkout xzz2l2nu_76x_dev
    git merge xzz2l2nu_76x
    git push origin xzz2l2nu_76x_dev
    ```

12. Make a PR from branch xzz2l2nu_76x_dev in your own respository to branch xzz2l2nu_76x in MMHY respository to let others cross-check your changes. Once looks good, merge it.

  The PR can be created on the webpage of your own repository:

      https://github.com/<your own github name>/cmgtools-lite/tree/xzz2l2nu_76x_dev

  E.g. A PR from hengne's xzz2l2nu_76x_dev branch to MMHY's xzz2l2nu_76x will look like this in the following link:

      https://github.com/mmhy/cmgtools-lite/compare/xzz2l2nu_76x...hengne:xzz2l2nu_76x_dev  


**Let's make it a rule here: Please always let other collabrators to view and sign your PR before merging it, even if you have the permission to do it all by yourself.**
  

Instruction to run jobs 
---------------------------------

1. run single job interactively

 Example configuration files are in cfg/ folder, e.g.
  
  ```
  heppy mc_test run_xzz2l2nu_76x_cfg_loose_mc.py
  ```
 this will run config file run_xzz2l2nu_76x_cfg_loose_mc.py and output to directory mc_test


2. run batch jobs on lsf

  Example scripts to submit lsf batch jobs are:
  ```
    cfg/loose_lsf_dt/sub_xzz2l2nu_76x_cfg_loose_dt.sh 
    cfg/loose_lsf_mc/sub_xzz2l2nu_76x_cfg_loose_mc.sh 
  ```
  for data and mc respectively.

  LSF jobs can only be submitted on lxplus, your local machine doesn't work. 

  One can then go to the output directory ('LSF/' in above example) to check the status:
  ```
   heppy_check.py *
  ```

  If you see some jobs are finished but not showing the results here, you can resub the jobs by running:
  ```
   heppy_check.py * -b 'bsub -q 8nh'
  ```
  which will resub the jobs with missing outputs.

  Once the jobs are all done, you can merge the outputs by running:
  ```
   heppy_hadd.py .
  ```
  this will collect all "Chunk"s of one sample and merge them into one folder.


Instruction to run jobs 
---------------------------------
  
  A ready to use automated scripts is provided:
  ```
   scripts/stack_dtmc.py
  ```
  In side you can see some pre-configured cutChains:
  ```
    cutChain='loosecut'
    #cutChain='tightzpt100'
    #cutChain='tightzpt100met100'
    #cutChain='tightzpt100met100dphi'
    #cutChain='tightmet250dphi'
  ```
  you can find the definition of these cutChains in the codes. 

  To draw the plots, un-comment the one you want to use, such as the "loosecut", and run:
  ```
   ./stack_dtmc.py 
  ```
  This will read the root trees I have put at:
  ```
   /afs/cern.ch/work/h/heli/public/XZZ/76X/
  ```
  and draw all the plots, which will then store in sub-directory "plots/".

  All plots are merged into one single .pdf file, such as "plots/loosecut_log_.pdf"


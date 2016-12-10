import _ from 'lodash';

import ubuntuEnvs from './ubuntu';
import centosEnvs from './centos';

let linuxEnvs = _.concat(centosEnvs);
export default linuxEnvs;

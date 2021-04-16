import { execSync } from 'child_process';

const emcompile = (args) => {
  const compileOptions = (args[2] && args[2] !== '.')
    ? `-${args[2]}`
    : '';

  const exportModule = (args[3] === 'export')
    ? '-s EXPORT_ES6=1 -s MODULARIZE=1'
    : '';

  const compileFunctions = args.slice(4)
    .reduce((str, funcName) =>
      str += `, '_${funcName}'`
      , ''
    );

  const cmd = `em++ ${args[0]} -o ${args[1]} ${compileOptions} ${exportModule} -s WASM=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap'] -s EXPORTED_FUNCTIONS="['_main'${compileFunctions}]"`;

  execSync(cmd, { encoding: 'utf-8' });
};

try {
  const args = process.argv.slice(2);
  emcompile(args);

} catch (err) {
  console.err(err);
}
